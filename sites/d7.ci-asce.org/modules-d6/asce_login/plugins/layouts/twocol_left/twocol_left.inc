<?php
// $Id: twocol_stacked.inc,v 1.1.2.3 2010/06/22 15:54:25 merlinofchaos Exp $

// Plugin definition
$plugin = array(
  'title' => t('Two column left column wider'),
  'category' => t('Columns: 2'),
  'icon' => 'twocol_left.png',
  'theme' => 'panels_twocol_left',
  'css' => 'twocol_left.css',
  'panels' => array(
    'top' => t('Top'),
    'left' => t('Left side'),
    'right' => t('Right side'),
    'bottom' => t('Bottom')
  ),
);
