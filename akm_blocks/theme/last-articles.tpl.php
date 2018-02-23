<?php
/**
 * @file
 * Default output for 'Latest from our blog block' articles.
 *
 * @author WebCodin <info@webcodin.com>
 *
 *  $title
 *  $summary
 *  $author
 *  $url
 */
?>
<article class="news-content">
  <h2><?php print $title; ?></h2>

  <p class="news-author"><?php print $author; ?></p>

  <div class="news-descr">
    <p><?php print $summary; ?></p>
  </div>
  <a href="<?php print $url; ?>" title="Read More" class="btn-base read-more">Read More</a>
</article>