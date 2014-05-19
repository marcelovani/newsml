<?php
/**
 * @file Contains the node NewsML row style plugin.
 */

/**
 * Plugin which performs a node_view on the resulting object
 * and formats it as an NewsML item.
 */

class views_plugin_row_node_newsml extends views_plugin_row {
  // Basic properties that let the row style follow relationships.
  var $base_table = 'node';
  var $base_field = 'nid';

  // Stores the nodes loaded with pre_render.
  var $nodes = array();

  function pre_render($values) {
    $nids = array();
    foreach ($values as $row) {
      $nids[] = $row->{$this->field_alias};
    }
    if (!empty($nids)) {
      $this->nodes = node_load_multiple($nids);
    }
  }

  function render($row) {
    // For the most part, this code is taken from node_feed() in node.module.
    global $base_url;

    $nid = $row->{$this->field_alias};
    if (!is_numeric($nid)) {
      return;
    }

    // Load the specified node:
    $node = $this->nodes[$nid];
    if (empty($node)) {
      return;
    }

    $body = field_get_items('node', $node, 'body');
    $body = trim($body[0]['safe_value']);
    $body .= "\n\nMore on " . l($node->title, 'node/'. $node->nid, array('attributes' => array('target'=>'_blank'), 'absolute' => TRUE));
    // Ignoring paging! It's a headache.
    $body = array($this->_clean_html(_filter_autop($body)));

    $item = new stdClass;
    $item->body = $body;
    $item->title = $this->html_convert_entities(htmlspecialchars($node->title));
    $item->link = url("node/$node->nid", array('absolute' => TRUE));
    $item->nid = $node->nid;
    $item->node = $node;

    return theme($this->theme_functions(),
      array(
        'view' => $this->view,
        'options' => $this->options,
        'row' => $item,
      )
    );
  }

  private function _clean_html($html) {
    global $base_url;
    //remove newlines and carriage returns.
    $html = str_replace(array("\r\n", "\n", "\r"), ' ', trim($html));

    //body text seems to have allready been UTF-8 encoded. Need to check this out in Page class.
    //$html = utf8_encode($html);

    //strip tags.
    $html = strip_tags($html,'<p><a><ul>');

    //remove attributes from <p> tags.
    //some attributes could contain '>' regex needs rethink.
    $html = preg_replace('/<p[^>]+>/', '<p>', $html);

    //strip empty tags.
    $html = preg_replace('/<p>\s*<\/p>/', '', $html);

    //strip empty space at begining of paragraph.
    $html = preg_replace('/<p>\s*/', '<p>', $html);

    //remove anchor tags.
    $html = preg_replace("/<a[^>]*?href=[\"|']#.*?<\/a>/i", "", $html);

    //convert relative URL's to absolute in anchor tags.
    $html = preg_replace("#href=(\"|')/|../../../#i", "href=$1" . $base_url . "/", $html);

    //remove <a> tags that lead to other sites.
    $html = preg_replace_callback("/<a[^>]*?href=[\"|'].*?:\/\/(.*?)[\"|'].*?>(.*?)<\/a>/", array(__CLASS__, 'strip_external_links'), $html);

    // replace target="" attributes.
    //$html = preg_replace('/target="_[a-z]+?"/', '', $html);

    $html = $this->html_convert_entities($html);

    return $html;
  }

  public static function strip_external_links($matches) {
    global $base_url;
    $host = str_replace('http://', '', $base_url);
    $capturedHost = explode('/', $matches[1]);
    $capturedHost = $capturedHost[0];
    if ($capturedHost != $host) {
      return $matches[2];
    }
    else {
      return $matches[0];
    }
  }

  private function html_convert_entities($string) {
    return preg_replace_callback('/&([a-zA-Z][a-zA-Z0-9]+);/', array(__CLASS__, 'convert_entity'), $string);
  }

  /* Swap HTML named entity with its numeric equivalent. If the entity
   * isn't in the lookup table, this function returns a blank, which
  * destroys the character in the output - this is probably the
  * desired behaviour when producing XML.
  */

  public static function convert_entity($matches) {
    // Entity not found? Destroy it.
    return isset(self::$table[$matches[1]]) ? self::$table[$matches[1]] : '';
  }

  static $table = array('quot'    => '&#34;',
    'amp'      => '&#38;',
    'lt'       => '&#60;',
    'gt'       => '&#62;',
    'OElig'    => '&#338;',
    'oelig'    => '&#339;',
    'Scaron'   => '&#352;',
    'scaron'   => '&#353;',
    'Yuml'     => '&#376;',
    'circ'     => '&#710;',
    'tilde'    => '&#732;',
    'ensp'     => '&#8194;',
    'emsp'     => '&#8195;',
    'thinsp'   => '&#8201;',
    'zwnj'     => '&#8204;',
    'zwj'      => '&#8205;',
    'lrm'      => '&#8206;',
    'rlm'      => '&#8207;',
    'ndash'    => '&#8211;',
    'mdash'    => '&#8212;',
    'lsquo'    => '&#8216;',
    'rsquo'    => '&#8217;',
    'sbquo'    => '&#8218;',
    'ldquo'    => '&#8220;',
    'rdquo'    => '&#8221;',
    'bdquo'    => '&#8222;',
    'dagger'   => '&#8224;',
    'Dagger'   => '&#8225;',
    'permil'   => '&#8240;',
    'lsaquo'   => '&#8249;',
    'rsaquo'   => '&#8250;',
    'euro'     => '&#8364;',
    'fnof'     => '&#402;',
    'Alpha'    => '&#913;',
    'Beta'     => '&#914;',
    'Gamma'    => '&#915;',
    'Delta'    => '&#916;',
    'Epsilon'  => '&#917;',
    'Zeta'     => '&#918;',
    'Eta'      => '&#919;',
    'Theta'    => '&#920;',
    'Iota'     => '&#921;',
    'Kappa'    => '&#922;',
    'Lambda'   => '&#923;',
    'Mu'       => '&#924;',
    'Nu'       => '&#925;',
    'Xi'       => '&#926;',
    'Omicron'  => '&#927;',
    'Pi'       => '&#928;',
    'Rho'      => '&#929;',
    'Sigma'    => '&#931;',
    'Tau'      => '&#932;',
    'Upsilon'  => '&#933;',
    'Phi'      => '&#934;',
    'Chi'      => '&#935;',
    'Psi'      => '&#936;',
    'Omega'    => '&#937;',
    'alpha'    => '&#945;',
    'beta'     => '&#946;',
    'gamma'    => '&#947;',
    'delta'    => '&#948;',
    'epsilon'  => '&#949;',
    'zeta'     => '&#950;',
    'eta'      => '&#951;',
    'theta'    => '&#952;',
    'iota'     => '&#953;',
    'kappa'    => '&#954;',
    'lambda'   => '&#955;',
    'mu'       => '&#956;',
    'nu'       => '&#957;',
    'xi'       => '&#958;',
    'omicron'  => '&#959;',
    'pi'       => '&#960;',
    'rho'      => '&#961;',
    'sigmaf'   => '&#962;',
    'sigma'    => '&#963;',
    'tau'      => '&#964;',
    'upsilon'  => '&#965;',
    'phi'      => '&#966;',
    'chi'      => '&#967;',
    'psi'      => '&#968;',
    'omega'    => '&#969;',
    'thetasym' => '&#977;',
    'upsih'    => '&#978;',
    'piv'      => '&#982;',
    'bull'     => '&#8226;',
    'hellip'   => '&#8230;',
    'prime'    => '&#8242;',
    'Prime'    => '&#8243;',
    'oline'    => '&#8254;',
    'frasl'    => '&#8260;',
    'weierp'   => '&#8472;',
    'image'    => '&#8465;',
    'real'     => '&#8476;',
    'trade'    => '&#8482;',
    'alefsym'  => '&#8501;',
    'larr'     => '&#8592;',
    'uarr'     => '&#8593;',
    'rarr'     => '&#8594;',
    'darr'     => '&#8595;',
    'harr'     => '&#8596;',
    'crarr'    => '&#8629;',
    'lArr'     => '&#8656;',
    'uArr'     => '&#8657;',
    'rArr'     => '&#8658;',
    'dArr'     => '&#8659;',
    'hArr'     => '&#8660;',
    'forall'   => '&#8704;',
    'part'     => '&#8706;',
    'exist'    => '&#8707;',
    'empty'    => '&#8709;',
    'nabla'    => '&#8711;',
    'isin'     => '&#8712;',
    'notin'    => '&#8713;',
    'ni'       => '&#8715;',
    'prod'     => '&#8719;',
    'sum'      => '&#8721;',
    'minus'    => '&#8722;',
    'lowast'   => '&#8727;',
    'radic'    => '&#8730;',
    'prop'     => '&#8733;',
    'infin'    => '&#8734;',
    'ang'      => '&#8736;',
    'and'      => '&#8743;',
    'or'       => '&#8744;',
    'cap'      => '&#8745;',
    'cup'      => '&#8746;',
    'int'      => '&#8747;',
    'there4'   => '&#8756;',
    'sim'      => '&#8764;',
    'cong'     => '&#8773;',
    'asymp'    => '&#8776;',
    'ne'       => '&#8800;',
    'equiv'    => '&#8801;',
    'le'       => '&#8804;',
    'ge'       => '&#8805;',
    'sub'      => '&#8834;',
    'sup'      => '&#8835;',
    'nsub'     => '&#8836;',
    'sube'     => '&#8838;',
    'supe'     => '&#8839;',
    'oplus'    => '&#8853;',
    'otimes'   => '&#8855;',
    'perp'     => '&#8869;',
    'sdot'     => '&#8901;',
    'lceil'    => '&#8968;',
    'rceil'    => '&#8969;',
    'lfloor'   => '&#8970;',
    'rfloor'   => '&#8971;',
    'lang'     => '&#9001;',
    'rang'     => '&#9002;',
    'loz'      => '&#9674;',
    'spades'   => '&#9824;',
    'clubs'    => '&#9827;',
    'hearts'   => '&#9829;',
    'diams'    => '&#9830;',
    'nbsp'     => '&#160;',
    'iexcl'    => '&#161;',
    'cent'     => '&#162;',
    'pound'    => '&#163;',
    'curren'   => '&#164;',
    'yen'      => '&#165;',
    'brvbar'   => '&#166;',
    'sect'     => '&#167;',
    'uml'      => '&#168;',
    'copy'     => '&#169;',
    'ordf'     => '&#170;',
    'laquo'    => '&#171;',
    'not'      => '&#172;',
    'shy'      => '&#173;',
    'reg'      => '&#174;',
    'macr'     => '&#175;',
    'deg'      => '&#176;',
    'plusmn'   => '&#177;',
    'sup2'     => '&#178;',
    'sup3'     => '&#179;',
    'acute'    => '&#180;',
    'micro'    => '&#181;',
    'para'     => '&#182;',
    'middot'   => '&#183;',
    'cedil'    => '&#184;',
    'sup1'     => '&#185;',
    'ordm'     => '&#186;',
    'raquo'    => '&#187;',
    'frac14'   => '&#188;',
    'frac12'   => '&#189;',
    'frac34'   => '&#190;',
    'iquest'   => '&#191;',
    'Agrave'   => '&#192;',
    'Aacute'   => '&#193;',
    'Acirc'    => '&#194;',
    'Atilde'   => '&#195;',
    'Auml'     => '&#196;',
    'Aring'    => '&#197;',
    'AElig'    => '&#198;',
    'Ccedil'   => '&#199;',
    'Egrave'   => '&#200;',
    'Eacute'   => '&#201;',
    'Ecirc'    => '&#202;',
    'Euml'     => '&#203;',
    'Igrave'   => '&#204;',
    'Iacute'   => '&#205;',
    'Icirc'    => '&#206;',
    'Iuml'     => '&#207;',
    'ETH'      => '&#208;',
    'Ntilde'   => '&#209;',
    'Ograve'   => '&#210;',
    'Oacute'   => '&#211;',
    'Ocirc'    => '&#212;',
    'Otilde'   => '&#213;',
    'Ouml'     => '&#214;',
    'times'    => '&#215;',
    'Oslash'   => '&#216;',
    'Ugrave'   => '&#217;',
    'Uacute'   => '&#218;',
    'Ucirc'    => '&#219;',
    'Uuml'     => '&#220;',
    'Yacute'   => '&#221;',
    'THORN'    => '&#222;',
    'szlig'    => '&#223;',
    'agrave'   => '&#224;',
    'aacute'   => '&#225;',
    'acirc'    => '&#226;',
    'atilde'   => '&#227;',
    'auml'     => '&#228;',
    'aring'    => '&#229;',
    'aelig'    => '&#230;',
    'ccedil'   => '&#231;',
    'egrave'   => '&#232;',
    'eacute'   => '&#233;',
    'ecirc'    => '&#234;',
    'euml'     => '&#235;',
    'igrave'   => '&#236;',
    'iacute'   => '&#237;',
    'icirc'    => '&#238;',
    'iuml'     => '&#239;',
    'eth'      => '&#240;',
    'ntilde'   => '&#241;',
    'ograve'   => '&#242;',
    'oacute'   => '&#243;',
    'ocirc'    => '&#244;',
    'otilde'   => '&#245;',
    'ouml'     => '&#246;',
    'divide'   => '&#247;',
    'oslash'   => '&#248;',
    'ugrave'   => '&#249;',
    'uacute'   => '&#250;',
    'ucirc'    => '&#251;',
    'uuml'     => '&#252;',
    'yacute'   => '&#253;',
    'thorn'    => '&#254;',
    'yuml'     => '&#255;',
  );
}
