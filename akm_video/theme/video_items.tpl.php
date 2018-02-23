<?php
/**
 * @file
 * Output for video items
 *
 * Avaliable variables
 * $items
 */
?>
<?php if (!empty($items)) : ?>
  <div class="box-model video-content">
    <div class="row">
      <?php foreach ($items as $item) : ?>
        <div class="field-video-item col-md-4 col-sm-6">
          <?php print render($item['video']); ?>
          <div class="field-user-name"><p><?php print l($item['name'], 'user/' . $item['uid']); ?></p></div>
          <div class="field-user-phone"><p><?php print $item['phone']; ?></p></div>
        </div>
      <?php endforeach; ?>
    </div>
  </div>
<?php else: ?>
  <div class="view-empty box-model">
    <h2>Sorry, nothing to find. Try change search criteria!</h2>
  </div>
<?php endif; ?>
