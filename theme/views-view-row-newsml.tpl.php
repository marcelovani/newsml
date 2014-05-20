<?php
/**
 * @file views-view-row-newsml.tpl.php
 * Default view template to display a item in an NewsML feed.
 *
 * @ingroup views_templates
 */
?>
<NewsItem>
  <Identification>
    <NewsIdentifier>
      <ProviderId><?php print $provider ?></ProviderId>
    </NewsIdentifier>
  </Identification>
  <NewsManagement>
    <NewsItemType FormalName="News"/>
    <FirstCreated><?php print $created ?></FirstCreated>
    <ThisRevisionCreated><?php print $changed ?></ThisRevisionCreated>
    <Status FormalName="Usable"/>
  </NewsManagement>
  <NewsComponent Duid="<?php print $row->nid ?>" Essential="no" EquivalentsList="no">
    <NewsLines>
      <CopyrightLine/>
    </NewsLines>
    <NewsComponent Duid="body-<?php print $row->nid ?>">
      <DateLine><?php print $published ?></DateLine>
<?php if (isset($author) && !empty($author)): ?>
      <Author><?php print $author ?></Author>
<?php endif ?>
      <NewsComponent>
        <Role FormalName="Main"/>
        <NewsLines>
          <HeadLine><?php print $row->title ?></HeadLine>
<?php if (isset($teaser) && !empty($teaser)): ?>
          <SlugLine><?php print $teaser ?></SlugLine>
<?php endif ?>
          <MoreLink><?php print url('node/' . $row->nid, array('absolute' => TRUE)) ?></MoreLink>
        </NewsLines>
        <ContentItem>
          <MediaType FormalName="Text"/>
          <MimeType FormalName="text/vnd.IPTC.NITF"/>
          <DataContent>
<?php foreach ($row->body as $page): ?>
            <nitf>
              <body>
                <body.head>
                  <hedline>
                    <hl1><?php print $row->title ?></hl1>
                  </hedline>
                </body.head>
                <body.content>
                  <?php print $page ?>
                </body.content>
              </body>
            </nitf>
<?php endforeach ?>
          </DataContent>
        </ContentItem>
<?php if (isset($tags) && !empty($tags)): ?>
        <item_keywords>
<?php foreach ($tags as $tag): ?>
          <item_keyword><?php print htmlspecialchars($tag, ENT_NOQUOTES); ?></item_keyword>
<?php endforeach ?>
        </item_keywords>
<?php endif ?>
      </NewsComponent>
<?php if (isset($image)): ?>
      <NewsComponent>
        <Role FormalName="Supporting"/>
        <NewsComponent Duid="main-image-<?php print $row->nid ?>">
          <NewsComponent>
            <Role FormalName="Main"/>
            <ContentItem
              Href="<?php print $image->path ?>"
              <?php if (isset($image->credits)) echo ' copyright="&#174;' . $image->credits . '&#174;"' ?>
              <?php if (isset($image->description)) echo ' imagetext="' . $image->description . '"' ?>
              <?php if (isset($image->title)) echo ' title="' . $image->title . '"' ?>>
              <MediaType FormalName="Photo"/>
              <MimeType FormalName="<?php print $image->filemime ?>"/>
            </ContentItem>
          </NewsComponent>
          <NewsComponent>
            <Role FormalName="Caption"/>
            <ContentItem>
              <MediaType FormalName="Text"/>
              <MimeType FormalName="text/plain"/>
              <DataContent/>
            </ContentItem>
          </NewsComponent>
        </NewsComponent>
      </NewsComponent>
<?php endif ?>
    </NewsComponent>
  </NewsComponent>
</NewsItem>
