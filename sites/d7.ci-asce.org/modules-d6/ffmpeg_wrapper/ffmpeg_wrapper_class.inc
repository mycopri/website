<?php

// $Id: ffmpeg_wrapper_class.inc,v 1.1.2.16 2011/01/20 18:03:55 arthuregg Exp $

/**
 * @file
 * The base class for media mover steps
 */
class ffmpeg_wrapper {

  /**
   * Construct a new ffmpeg object
   *
   * @param $command
   *   String, default command to run
   */
  function __construct($command = NULL) {
    // Initialize errors
    $this->errors = array();
    // Do we have a set path?
    if (! is_executable(variable_get('ffmpeg_wrapper_path', '/dev/null'))) {
      if (! ffmpeg_wrapper_which_ffmpeg($this)) {
        return FALSE;
      }
    }
    // Set the path
    $this->path = variable_get('ffmpeg_wrapper_path', '/dev/null');

    $this->vhook_path = variable_get('ffmpeg_wrapper_vhook', '/usr/local/lib/vhook');
    if ($command) {
      $this->command($command);
    }

  }

  /**
   * Runs the requested command
   */
  function command($command, $verbosity = -1) {
    // Check and see if anyone is overridding this
    if ($function = variable_get('ffmpeg_wrapper_engine', FALSE)) {
      if (function_exists($function)) {
        return $function($command, $this);
      }
    }

    // Purge any errors that a previous run may have created
    $this->errors = array();

    // Create some exceptions for dealing with pipes and semicolons.
    // This is done because os the shell escape
    $pattern = array('\;', '\|');
    $replace = array(';', '|');

    // Escape our command, but make sure that we allow | and ;
    $this->command = str_replace($pattern, $replace, escapeshellcmd($this->path . ' ' . $command)) . " -v $verbosity";

    // Find the input file
    preg_match("/-i.*?'(.*?)'/", $this->command, $matches);
    $this->input_file = $matches[1];

    // Find the output file in the command if there was one.
    // This is sort of hacky but is helpful for passing the file out.
    preg_match("/.*'(.*?)'.*?$/", $this->command, $matches);
    $this->output_file = ! empty($matches[1]) ? $matches[1] : NULL;

    // Keep track of where this is being executed
    $this->cwd = getcwd();

    $descriptor_spec = array(
      0 => array('pipe', 'r'),
      1 => array('pipe', 'w'),
      2 => array('pipe', 'w')
    );

    $pipes = array();
    $process = proc_open($this->command, $descriptor_spec, $pipes, $this->cwd, NULL, array('binary_pipes' => TRUE));

    if (is_resource($process)) {
      fclose($pipes[0]);
      $this->output = stream_get_contents($pipes[1]);
      fclose($pipes[1]);
      $output = stream_get_contents($pipes[2]);
      $this->output .= $output;
      fclose($pipes[2]);
      $command_return = proc_close($process);
    }

    // Parse errors
    $this->error_check();

    // Return the output
    return $this->output;
  }


  /**
   * Check the output from FFmpeg for various strings
   */
  function error_check() {
    // build the error conditions these are all pulled by hand at this point
    // @NOTE one has to be careful to have a specific match as there are strings
    // in the output from #ffmpeg which containg "error"
    $errors = array(
      '/Segmentation fault .*/i',
      '/Unsupported .*/i',
      // match: bad formats
      '/Unknown format .*/i',
      '/Unable for find a suitable output format for .*/i',
      '/Incorrect frame size .*/i',
      '/Unsupported codec .*/i',
      '/Could not write header .*/i',
      '/already exists. Overwrite/i',
      '/sh: [0-9a-zA-Z\/]*: not found .*/i',
      '/no such file or directory .*/i',
      '/.*does not support that sample rate.*/i',
      // match: Error while opening codec for output stream #0.1 - maybe incorrect parameters such as bit_rate, rate, width or height
      '/Error while opening codec for output stream.*/i',
      // match: I/O error occured
      '/I\/O error occured/i',
      '/Failed to add video hook function*/i',
      '/ffmpeg: unrecognized option*/i',
      '/Could not open */i',
    );

    // Check for error conditions in the FFmpeg output
    foreach ($errors as $error) {
      preg_match($error, $this->output, $matches);
      if (count($matches)) {
        $this->errors[] = print_r($matches[0], TRUE);
      }
    }

    // check to see that a viable file was created
    if (! empty($this->output_file)) {
      // Does the new file exists?
      if (! file_exists($this->output_file)) {
        $this->errors[] = t('FFmpeg failed to create an output file. This could be due to permission problems or an error from the encoding settings.');
      }
      // check to see that the file had data
      elseif (filesize($this->output_file) < 10 ) {
        $this->errors[] = t('FFmpeg created a file, but it does not have any data. This could be a problem with the encoding settings.');
      }
    }
  }


  /**
   * Returns the installed version of FFmpeg
   * @param $format
   *   one of string, version
   * @return $version
   *   String, FFmpeg version
   */
  function get_version($format = 'string') {
    // get string from ffmpeg and return it if it is enough
    $output = $this->command('-v');

    if ($format == 'string') {
      return $output;
    }

    // Search for SVN string
    // FFmpeg version SVN-r20438, Copyright (c) 2000-2009 Fabrice Bellard, et al.
    $pattern = "/ffmpeg version SVN-r([0-9.]*)/i";
    if (preg_match($pattern, $output, $matches)) {
      return array(
        'svn' => (int) $matches[1],
        'version' => (int) $matches[1],
      );
    }

    // Do we have a release?
    // ffmpeg version 0.4.9-pre1, build 4736, Copyright (c) 2000-2004 Fabrice Bellard
    $pattern = "/ffmpeg version ([0-9.]*)/i";
    if (preg_match($pattern, $output, $matches)) {
      return array(
        'release' => (float) $matches[1],
        'version' => (float) $matches[1]
      );
    }

    // Do we have a build version?
    // ffmpeg version 0.4.9-pre1, build 4736, Copyright (c) 2000-2004 Fabrice Bellard
    $pattern = "/ffmpeg version.*, build ([0-9]*)/i";
    if (preg_match($pattern, $output, $matches)) {
      return array(
        'build' => (float) $matches[1],
        'version' => (float) $matches[1],
      );
    }
  }


  /**
   * Acquires all of the codes in use
   * @return unknown_type
   */
  function get_codecs() {
    // Can we utilize cached data?
    if ($codecs = variable_get('ffmpeg_wrapper_codecs', FALSE)) {
      return $codecs;
    }

    $data = array();

    // We know where the codecs are by looking at the output of
    // ffmpeg -formats or ffmpeg -codecs depending of
    // version SVN-r > 20561 or version > 0.5
    $ffmpeg_version = $this->get_version('svn');
    if ($ffmpeg_version['svn'] >= 20561 || $ffmpeg_version['release'] > 0.5 ) {
      $codecs = $this->command('-codecs');
      // Remove the leading space
      $codecs_formats_pos = strpos($codecs, 'Codecs:') + 8;
      $codecs_formats_pos_end = strpos($codecs, 'Note,');
    }
    else {
      $codecs = $this->command('-formats');
      // Remove the leading space
      $codecs_formats_pos = strpos($codecs, 'Codecs:') + 8;
      $codecs_formats_pos_end = strpos($codecs, 'Supported file protocols:');
    }
    $codecs = substr($codecs, $codecs_formats_pos, ($codecs_formats_pos_end - $codecs_formats_pos));

    // Convert to array
    $codecs = explode("\n", $codecs);

    $rows = array();
    foreach ($codecs as $codec) {
      // match the decode, encode, type, S|D|T options (see: http://lists.mplayerhq.hu/pipermail/ffmpeg-user/2006-January/002003.html)
      // S = Draw horizontal band (decoder can use the draw_horiz_band callback)
      // D = DR1 (uses direct rendering method 1)
      // T = Truncated (input bitstream might be truncated at a random location instead of on frame boundary)

      // name
      $pattern ='/[ ]*([D ])([E ])([ VA])([S ])([ D])([ T])[ ]*([a-zA-Z0-9_,]*)[ ]*([a-zA-Z0-9,_ ]*)/';
      preg_match($pattern, $codec, $matches);
      list ($decode, $encode, $type, $s, $d, $t, $format) = array_slice($matches, 1);
      if ($type) {
        $data[$format] = array(
          'format' => $format,
          'type' => ($type == 'A' ? t('Audio') : t('Video')),
          'encode' => ($encode == 'E' ? TRUE : FALSE),
          'decode' => ($decode == 'D' ? TRUE : FALSE),
        );
      }
    }
    $this->codecs = $data;
    variable_set('ffmpeg_wrapper_codecs', $data);
    return $data;
  }

  /**
   * Acquire formats that can be processed
   * @return array
   */
  function get_formats() {
    // Can we utilize cached data?
    if ($formats = variable_get('ffmpeg_wrapper_formats', FALSE)) {
    //  return $formats;
    }

    $formats = $this->command('-formats');

    // slice up the format output
    $startpos = strpos($formats, 'File formats:');
    // depending of version SVN-r > 20561 or version > 0.5 string changes
    $ffmpeg_version = $this->get_version('svn');
    if ($ffmpeg_version['svn'] >= 20561 || $ffmpeg_version['release'] > 0.5) {
      $endpos = strpos($formats, 'FFmpeg version');
    }
    else {
      $endpos = strpos($formats, 'Codecs:');
    }
    $formats = substr($formats, $startpos, $endpos - $startpos);
    //remove the header
    $formats = str_replace('File formats:', '', $formats);

    $rows = array();
    $data = array();
    foreach (explode("\n", $formats) as $format) {
      // match the decode, encode, format, description
      $pattern ='/[ ]*([D ])([E ])[ ]*([a-zA-Z0-9_,]*)[ ]*([^\$]*)/';
      preg_match($pattern, $format, $matches);
      // Get the matches into variable format, note that $matches[0] is not wanted
      list ($decode, $encode, $types, $description) = array_slice($matches, 1);
      // There can be multiple types per format
      $types = explode(',', $types);
      foreach ($types as $type) {
        if ($type) {
          if (! $data[$type]) {
            $data[$type] = array();
          }
          $data[$type]['format'] = $type;
          $data[$type]['description'] = $description;

          // It can be the case that we have duplicate types
          // provided different codecs. Make sure that we do
          // not overwrite an existing 'Yes'
          if (! ($data[$type]['decode'] && $decode != 'D')) {
            $data[$type]['decode'] = ($decode == 'D' ? TRUE : FALSE);
          }
          if (! ($data[$type]['encode'] && $encode != 'E')) {
            $data[$type]['encode'] = ($encode == 'E' ? TRUE : FALSE);
          }
        }
      }
    }

    $this->formats = $data;
    variable_set('ffmpeg_wrapper_formats', $data);
    return $data;
  }


  /**
   * Return meta data for this file
   *
   * @param $filepath
   *   String, path to file
   * @return $file
   *   Array, contains meta data
   */
  function get_file_data($filepath) {
    if (file_exists($filepath)) {
      $this->command("-i \"$filepath\"");

      // Get file format
      $pattern = '/Input #0, (.*),/';
      preg_match($pattern, $this->output, $matches);
      $file['format'] = !empty($matches[1]) ? $matches[1] : 'na';

      // get file duration
      $file['duration'] = $this->get_file_duration(NULL, NULL, $this->output);

      // get bit rate
      $pattern = "/bitrate: ([0-9].*\/s)/";
      preg_match($pattern, $this->output, $matches);
      $file['bitrate'] = !empty($matches[1]) ? $matches[1] : 'na';

      // get audio settings
      // format is: codec, sample rate, stereo/mono, bitrate
      $pattern = "/Audio: (.*), ([0-9]*) Hz, (stereo|mono)/";
      preg_match($pattern, $this->output, $matches);
      $file['audio']['codec'] = !empty($matches[1]) ? $matches[1] : 'na';
      $file['audio']['ar'] = !empty($matches[2]) ? $matches[2] : 'na';
      $file['audio']['ac'] = (!empty($matches[3]) && $matches[3] == 'stereo') ? 2 : 1;

      // take the last match and extract the bit rate if present
      $pattern = "/Audio: .* (.*) kb\/s/";
      preg_match($pattern, $this->output, $matches);
      $file['audio']['ab'] = !empty($matches[1]) ? $matches[1] : 'na';

      // VIDEO ----------------------------------------
      // The formating of video can be difficult. We use different
      // patterns to look for the video information

      // try pattern that takes into account a codec's color space (example: yuv420p)
      // eg: Video: mpeg1video, yuv420p, 320x240 [PAR 1:1 DAR 4:3], 990 kb/s, 30.00 tb(r)
      // the above is: codec, color space, frame size, bitrate, frame rate
      $pattern1 = "/Video: ([^,]+), ([^,]+), ([0-9x]+)[^,]*, ([0-9]*.*\/s|[A-Za-z]+[^,]*), ([0-9\.]*)/";

      // pattern that omits video bitrate but not color space.
      // eg: Video: mpeg4, yuv420p, 640x480 [PAR 1:1 DAR 4:3], 23.98 tb(r)
      // the above is: codec, color space, frame size, frame rate
      $pattern2 = "/Video: ([^,]+), ([^0-9][^,]*), ([0-9x]+)[^,]*, ([0-9\.]*)/";

      // pattern that omits a codec's color space and video bitrate
      // eg: Video: mpeg4, 640x480, 29.97 tb(r)
      // the above is: codec, frame size, frame rate
      $pattern3 = "/Video: ([^,]+), ([0-9x]+)[^,]*, ([0-9\.]*)/";

      // pattern that looks for OSX specific output
      // eg: Stream #0.1(eng), 25.00 fps(r): Video: mpeg4, yuv420p, 768x432
      // the above is: frame rate, codec, colour space, frame size
      $pattern4 = "/Video: ([^,]+), ([^,]+), ([0-9x]+)[^,]/";

      // Search for the data
      if (preg_match($pattern1, $this->output, $matches)) {
        $file['video']['codec'] = !empty($matches[1]) ? $matches[1] : 'na';
        // $file['video']['type'] = $matches[2];
        $file['video']['s'] = !empty($matches[3]) ? $matches[3] : 'na';
        $file['video']['br'] = !empty($matches[4]) ? $matches[4] : 'na';
      }
      elseif (preg_match($pattern2, $this->output, $matches)) {
        $file['video']['codec'] = !empty($matches[1]) ? $matches[1] : 'na';
        $file['video']['s'] = !empty($matches[3]) ? $matches[3] : 'na';
        $file['video']['br'] = 'na';
      }
      elseif (preg_match($pattern3, $this->output, $matches)) {
        $file['video']['codec'] = !empty($matches[1]) ? $matches[1] : 'na';
        $file['video']['s'] = !empty($matches[3]) ? $matches[3] : 'na';
        $file['video']['br'] = 'na';
      }
      elseif (preg_match($pattern4, $this->output, $matches)) {
        $file['video']['codec'] = !empty($matches[1]) ? $matches[1] : 'na';
        $file['video']['s'] = !empty($matches[3]) ? $matches[3] : 'na';
      }
      // @TODO need debugging code here

      return $file;
    }
  }


  /**
   * Returns file duration
   * @param $path
   *   String, path to file
   * @param $timecode
   *   Boolean, return time format or unix time
   * @param $output
   *   String, output from FFmpeg, useful for not running FFmpeg again
   * @return $time
   *   String, unix time or timecode
   */
  function get_file_duration($path = null, $timecode = null, $output = null) {
    // Was output from FFmpeg passed in?
    if (! $output) {
      // Get duration from ffmpeg
      // Need quotes around the path parameter in case filename has spaces.
      $ffmpeg_output = ffmpeg_wrapper_run_command("-i \"$path\"");
      $this->command("-i \"$path\"");
      $output = $this->output;
    }

    // Parse the output looking for "Duration: 00:02:12"
    $pattern = "/Duration: ([0-9]+:[0-9]+:[0-9]+)\.[0-9]+/";
    preg_match($pattern, $output, $matches);
    if (! $time = $matches[1]) {
      return FALSE;
    }

    // Return duration in seconds?
    if (! $timecode) {
      // Get the time into an array
      $time = explode(':', $time);
      $seconds = 0;
      if ($time[0] != '00') {
        $seconds += $time[0] * 60 * 60;
      }
      if ($time[1] != '00') {
        $seconds += $time[1] * 60;
      }
      $seconds += $time[2];
      $time = $seconds;
    }
    return $time;
  }

  /**
   * Are the necessary codecs installed to decode this file?
   * @param $path
   *   String, path to file
   * @return
   *   Boolean
   */
  function can_decode_file($path) {
    // Get supported formats
    $formats = $this->get_formats();
    // Get the format and codec information on this file
    $file_data = $this->get_file_data($path);
     // If the file has multiple formats, all formats
     // must be in $file_types Otherwise, it is
     // not supported by ffmpeg.
    foreach(explode(',', $file_data['format']) as $type) {
      if (! $formats[$type]['decode']) {
        $this->errors[] = t('The file could not be transcoded because this version of FFmpeg can not decode this kind of file');
        return FALSE;
      }
    }

    // If we have na values for the audio and video codecs, the file can not be decoded
    // @TODO is this really TRUE? The logic below does not seem to indicate this,
    //       however, I might not understand what the 'na' value really means
    if ($file_data['audio']['codec'] == 'na' && $file_data['video']['codec'] == 'na') {
      $this->errors[] = t('The file could not be transcoded because the audio and video codes do not exist');
      return FALSE;
    }

    // Now get installed codecs to compare file codecs
    $codecs = $this->get_codecs();

    // Can we decode this audio type?
    if ($codecs[$file_data['audio']['codec']]['decode'] || $file_data['audio']['codec'] == 'na' ) {
      // Are we only handing back audio?
      if ($types['audio'] && ! $types['video']) {
        return TRUE;
      }
      $can_audio_decode = TRUE;
    }
    else {
      $this->errors[] = t('The file could not be transcoded because the audio codec is not supported');
    }

    // Can we decode this video type?
    if ($codecs[$file_data['video']['codec']]['decode'] || $file_data['video']['codec'] == 'na') {
      // are we only handing back video?
      if ($types['video'] && ! $types['audio']) {
        return TRUE;
      }
      $can_video_decode = TRUE;
    }
    else {
       $this->errors[] = t('The file could not be transcoded because the video codec is not supported');
    }

    // If we can decode both audio and video, the file is ok
    if ($can_audio_decode && $can_video_decode) {
      return TRUE;
    }

    // @TODO need additional debugging information here
    return FALSE;
  }


}