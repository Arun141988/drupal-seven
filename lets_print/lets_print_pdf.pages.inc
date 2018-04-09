<?php

/**
 * @file
 * Contains all description to generate PDF document from raw HTML content.
 */

/**
 * Generate HTML of a given node.
 *
 * @return bool
 *   TRUE if PDF is successfully generated and FALSE if it isn't.
 */
function lets_print_generate_pdf($node) {

  if (!node_access('view', $node)) {
    drupal_set_message(t('You are not authorized to generate PDF for this page.'), 'warning');
    drupal_goto(urlencode('node') . '/' . $node->nid);
    return;
  }
  if (empty($node)) {
    drupal_get_messages('error');
    drupal_set_message(t('PDF cannot be generated for this path.'), 'error');
    return;
  }

  // Checking mPDF library existence.
  if (lets_print_pdf_library_exist() == TRUE) {
    $lets_print_pdf_filename = variable_get('lets_print_pdf_filename');
    $lets_print_pdf_filename = token_replace($lets_print_pdf_filename, array('node' => $node));

    //$view = node_view($node);
    $view = node_view($node, 'PDF');
    $html = drupal_render($view);

    _lets_print_pdf_generator($html, $lets_print_pdf_filename, $node->nid);
  }
  else {
    drupal_set_message(t('mPDF library is not included. Please check your "sites/all/libraries" directory or " /sites/your-domain-name/libraries/ directory " or "!default_module_path" directory.', array(
      '!default_module_path' => drupal_get_path('module', 'lets_print'),
    )), 'warning');
    drupal_goto(urlencode('node') . '/' . $node->nid);
    return;
  }
}
