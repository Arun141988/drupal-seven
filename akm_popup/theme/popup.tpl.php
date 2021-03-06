<?php
/**
 * @file
 * Popup template
 *
 * Avaliable variables
 * $popup_title
 * $popup_content
 * $popup_button_text
 *
 */
?>
<div id="gce-popup" class="gce-popup-container" style="display: none;">

  <?php if (!empty($popup_title)): ?>
    <h2><?php print $popup_title; ?></h2>
  <?php endif; ?>

  <?php if (!empty($popup_content)): ?>
    <div><?php print $popup_content; ?></div>
  <?php endif; ?>

  <?php if (!empty($popup_button_text)): ?>
    <div class="popup-btn-container"><a href="#" class="btn-base agree-btn"><?php print $popup_button_text; ?></a></div>
  <?php endif; ?>

</div>