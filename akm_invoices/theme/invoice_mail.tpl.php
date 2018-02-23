<?php
/**
 * @file
 * Default output for a invoice mail node.
 *
 * Avaliable variables
 * $subject
 * $body
 *
 */
?>
<html>
<head>
  <title><?php print $subject; ?></title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
</head>
<body>
<?php print $body; ?>
</body>
</html>
