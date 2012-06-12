<?php
// $Id: threecol_33_34_33_stacked.inc,v 1.1.2.2 2010/06/22 15:54:25 merlinofchaos Exp $

// Plugin definition
$plugin = array(
  'title' => t('Committee'),
  'category' => t('Columns: 3'),
  'icon' => 'committee.png',
  'theme' => 'committee',
  'css' => 'committee.css',
  'panels' => array(
    'top' => t('Top'),
    'upper-left' => t('Upper Left side'),
    'upper-middle' => t('Upper Middle column'),
    'upper-right' => t('Upper Right side'),
    'lower-left' => t('Lower Left side'),
    'lower-middle' => t('Lower Middle column'),
    'lower-right' => t('Lower Right side'),
'bottom' => t('Bottom')
  ),
);
