<?php
/**
 * @file views-view-newsml.tpl.php
 * Default template for feed displays that use the NewsML style.
 *
 * @ingroup views_templates
 */
?>
<?xml version="1.0" encoding="utf-8"?>
<NewsML xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:exsl="http://exslt.org/common">
  <NewsEnvelope>
    <DateAndTime><?php print $datetime ?></DateAndTime>
  </NewsEnvelope>
  <?php if (!empty($options['logo'])): ?><NewsService FormalName="Thumbnail" link-url="<?php print $base_url ?>"><?php print $options['logo'] ?></NewsService><?php endif ?>

  <?php print $rows; ?>
</NewsML>