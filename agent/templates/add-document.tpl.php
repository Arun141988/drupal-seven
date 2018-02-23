<?php
$form = $variables['form'];
$output = '';
$theme_path = drupal_get_path('theme', 'bootstrap');
print render($form['form_id']);
print render($form['form_build_id']);
print render($form['form_token']);
$clientID=$form['clientid']['#default_value'];
?>
<div id="seccmsg"></div>
<div id="main">
    <div id="content">
        <div id="modal2" class="modalgh">
            <div class="modalgh-content">
                <h4 style="text-align:center;">Are you sure you want to delete <strong id="delfileName">this </strong> file?</h4>
            </div>
            <div class="modalgh-footer">
                <a class="modal-action modal-close waves-effect btn-flat waves-light btn btn-primary modalgh-btn">Not yet</a>
                <a id="delete-file" class="modal-action modal-close waves-effect btn-flat waves-light btn btn-primary modalgh-btn">Delete</a>
            </div>
        </div>
        <section class="profileforminn">            
            <div class="profileslide">                                      
                <div class="profilemidinn">
                    <div class="uni-detail-mid-white doc-main-visible">
                        <div class="uni-detail-mid-arti-innbg documentconbg">
                            <div class="uni-detail-mid-arti-innbg-row">
                                <div class="uni-detail-mid-arti-innbg-row-mid-left">
                                    <div class="uni-detail-mid-arti-innbg-row-mid-left-inn">
                                        <div class="uni-detail-mid-discription-con-lefttitle">To the insitution</div>
                                        <div class="documentconform-bg">
                                            <div class="documentconformupload-bg">
                                                <div class="documentconformupload-left" style="display:none;">Upload:</div>
                                                <div class="documentconformupload-right">
                                                    <div class="documentconformupload-right-inn">
                                                        <div class="file-field input-field">
                                                            <?php print render($form['field_student_documents']); ?>
                                                        </div>
                                                    </div>

                                                    <div style="width:100%; float:left; margin-bottom: 15px;">
                                                        <div class="document-rightbox-bg">
                                                            <?php 
                                                                print render($form['document_type']); 
                                                                print render($form['clientid']); 
                                                            ?>
                                                        </div>
                                                        <div class="document-rightbox-bg">
                                                            <?php 
                                                                print render($form['save']);                                                                 
                                                                print render($form['cancel']);
                                                            ?>                                                                            
                                                        </div>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class="tableboxbg">
                                                <table class="striped">
                                                    <thead>
                                                        <tr>
                                                            <th data-field="id">File Name</th>
                                                            <th data-field="name">Type</th>
                                                            <th data-field="price">Delete?</th>
                                                        </tr>
                                                    </thead>
                                                    <?php $file_data=agent_get_document('ALLAPPLICATION',$clientID); ?>
                                                    <tbody>

                                                        <?php foreach ($file_data as $key => $val) { ?>
                                                            <tr id="mydocuments-<?php print $val->fid; ?>"><td><a href="<?php print $val->fullpath; ?>" download><?php print $val->filename; ?></a></td><td><?php print ucfirst($val->DocumentTypeName); ?></td><td>        
                                                                    <?php
                                                                    if ($val->delete_access == 'YES') {
                                                                        print render($form['document_delete'][$val->fid]);
                                                                        ?>
                                                                        <a class="modal-trigger open-del-conf-modal" href="#modal2" rel="<?php print $val->fid; ?>"><img src="/<?php print $theme_path; ?>/images/delete-new.png" class="del-doc-btn"></a>
                                                                        <?php
                                                                    } else {
                                                                        $output.=$val->delete_access;
                                                                    }
                                                                    ?>
                                                                </td></tr>
                                                        <?php } ?>
                                                    </tbody>
                                                </table>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                                <?php                                
                                $app_file_data = agent_get_document('APPLICATIONSPECIFICS',$clientID);
                                if (is_array($app_file_data) && count($app_file_data) > 0) {
                                    ?>
                                    <div class="uni-detail-mid-arti-innbg-row-mid-right">
                                        <div class="uni-detail-mid-arti-innbg-row-mid-left-inn">
                                            <div class="uni-detail-mid-discription-con-lefttitle">From the Institution</div>
                                            <div class="documentconform-bg">
                                                <div class="tableboxbg">
                                                    <table class="striped">
                                                        <thead>
                                                            <tr>
                                                                <th data-field="id">File Name</th>
                                                                <th data-field="name">Type</th>
                                                                <th data-field="price"></th>
                                                            </tr>
                                                        </thead>
                                                        <tbody>
                                                            <?php
                                                            if (count($app_file_data) > 0) {
                                                                foreach ($app_file_data as $key => $val) {
                                                                    ?>
                                                                    <tr><td><a href="<?php print $val->fullpath; ?>" download><?php print $val->filename; ?></a></td><td><?php print ucfirst($val->DocumentTypeName); ?></td><td><?php print ucfirst($val->SugarCRMAppIA); ?></td></tr>
                                                                    <?php
                                                                }
                                                            }
                                                            ?>        
                                                        </tbody></table></div>
                                            </div>
                                        </div>
                                    </div>
                                <?php } ?>
                            </div>
                        </div>
                    </div>
                </div>                        
            </div>                                               
        </section>
    </div>
</div>

