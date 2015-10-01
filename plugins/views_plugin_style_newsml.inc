<?php
/**
 * @file
 * Contains the NewsML style plugin.
 */

/**
 * Default style plugin to render an NewsML feed.
 *
 * @ingroup views_style_plugins.
 */

class views_plugin_style_newsml extends views_plugin_style {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['provider'] = array(
      '#type' => 'textfield',
      '#title' => t('Provider'),
      '#default_value' => $this->options['provider'],
      '#description' => t('Provider ID for the feed.'),
    );
    $form['logo'] = array(
      '#type' => 'textfield',
      '#title' => t('Logo thumbnail'),
      '#default_value' => $this->options['logo'],
      '#description' => t('Logo dimensions should be 130x30pixels maximum.'),
    );
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['provider'] = array('default' => '', 'translatable' => TRUE);
    $options['logo'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function render() {
    if (empty($this->row_plugin)) {
      debug('views_plugin_style_default: Missing row plugin');
      return;
    }
    $rows = '';

    foreach ($this->view->result as $row_index => $row) {
      $this->view->row_index = $row_index;
      $rows .= $this->row_plugin->render($row);
    }

    $output = theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'rows' => $rows
      ));
    unset($this->view->row_index);
    return $output;
  }
}
