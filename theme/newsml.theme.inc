<?php

function template_preprocess_views_view_newsml(&$vars) {
  global $base_url;
  $vars['base_url'] = $base_url;
  $vars['datetime'] = date('Ymd\THis');

  if (empty($vars['view']->live_preview)) {
    drupal_add_http_header('Content-Type', 'application/xml; charset=utf-8');
  }
}

function template_preprocess_views_view_row_newsml(&$vars) {
  global $base_url;
  $vars['base_url'] = $base_url;
  $vars['provider'] = token_replace($vars['view']->style_plugin->options['provider']);

  $node = $vars['row']->node;
  $vars['created'] = date('Ymd\THis', $node->created);
  $vars['changed'] = date('Ymd\THis', $node->changed);
  $published = field_get_items('node', $node, 'field_published_date');
  if (!empty($published)) {
    $vars['published'] = date('Ymd\THis', strtotime($published[0]['value']));
  }

  $authors = field_get_items('node', $node, 'field_author');
  if (isset($authors[0]['node']->title)) {
    $vars['author'] = $authors[0]['node']->title;
  }
  elseif (isset($authors[0]['nid'])) {
    $author = node_load((int) $authors[0]['nid']);
    $vars['author'] = $author->title;
  }

  $teaser = field_get_items('node', $node, 'field_short_teaser');
  if (!empty($teaser)) {
    $vars['teaser'] = $teaser[0]['safe_value'];
  }
  $tags = field_get_items('node', $node, 'field_tags');
  if (!empty($tags)) {
    foreach ($tags as $tag) {
      $term = taxonomy_term_load($tag['tid']);
      if ($term) {
        $vars['tags'][] = $term->name;
      }
    }
  }
  $images = field_get_items('node', $node, 'field_primary_image');
  if (!empty($images)) {
    $image = new stdClass();
    $image->path = image_style_url('newsml_feed', $images[0]['uri']);
    $image->filemime = $images[0]['filemime'];
    $vars['image'] = $image;
  }
}