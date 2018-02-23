<?php
$form = $variables['form'];
$clientID = $form['clientID']['#default_value'];
if(intval($clientID)>0){
    $check_array = agent_checking_application_form_step($clientID);
}
$output = '';
$popup_text = '';
$theme_path = drupal_get_path('theme', 'bootstrap');
$num_courses = count(element_children($form['courses']));
$ossc_application_apply_cofirm_text = variable_get('ossc_application_apply_cofirm_text', NULL);
print render($form['form_id']);
print render($form['form_build_id']);
print render($form['form_token']);
?>
<a class="modal-trigger" href="#modal1"><p id="declaration-modal"></p></a>
<div id="modal1" class="modalgh">
    <div class="modalgh-content">
        <?php print $ossc_application_apply_cofirm_text['value']; ?>
    </div>
    <div class="modalgh-footer">
        <a class="modal-action modal-close waves-effect btn-flat waves-light btn btn-primary modalgh-btn">Not ready yet</a>
        <a class="modal-action modal-close waves-effect btn-flat apply-app waves-light btn btn-primary modalgh-btn">Apply</a>
    </div>
</div>
<?php
    if ($check_array['step1']['status'] == 1 || $check_array['step3']['status'] == 1) { ?>
        <span class="plus-addicon-newicon"><a href="#modal2" class="btn-floating btn-large waves-effect waves-light red modal-trigger"><i class="material-icons">add</i></a></span>
        <?php
        $popup_text = t('Please first complete the required profile/document information.');
    } else { ?>
        <span id="waves-effect-new" class="plus-addicon-newicon"><a class="btn-floating btn-large waves-effect waves-light red"><i class="material-icons">add</i></a></span>
        <?php
    }
?>
<div id="seccmsg"></div>
<div id="main">
    <a class="modal-trigger" href="#modal4"><p id="app-required"></p></a>
    <div id="modal4" class="modalgh">
        <div class="modalgh-content">
            <!--h4>Test Title</h4-->
            <p><h4 style="text-align:center;"><?php print t('Please select required field.'); ?></h4></p>
        </div>
        <div class="modalgh-footer">
            <a class="modal-action modal-close waves-effect waves-red btn-flat waves-light btn btn-primary modalgh-btn">Ok</a>
        </div>
    </div>
    <div id="modal2" class="modalgh">
        <div class="modalgh-content">
            <h4 style="text-align:center;"><?php print $popup_text; ?></h4>
        </div>
        <div class="modalgh-footer">
            <a class="modal-action modal-close waves-effect waves-red btn-flat waves-light btn btn-primary modalgh-btn">Ok</a>
        </div>
    </div>
    <div id="content">
        <?php
        ($check_array['step2']['status'] == 1) ? $addapp_btn_msg = t('Click on the add button below to add an application') : $addapp_btn_msg = '';
        if ($addapp_btn_msg != '') {
            if (!(isset($_GET['id']) && $_GET['id'] != '')) {
                $output.='<div class="addapp-btn-msg" id="addapp-btn-msg">' . $addapp_btn_msg . '</div>';
            }
        }
        $num_courses = count(element_children($form['courses']));
        ?>
        <section class="profileforminn">    
            <div class="profileslide">
            <div class="profilemidinn">
                <div style="margin: 10px; color: #000;">
                    <ul class="collapsible popout collapsible-accordion" data-collapsible="accordion">                        
                        <?php print render($form['clientID']); ?>
                        <?php
                        $j = 0;
                        for ($i = 0; $i < $num_courses; $i++) {
                            $status = '';
                            $status = $form['courses'][$i]['status']['#default_value'];
                            if ($status) {
                                $status = showAppStatus($status);
                            } else {
                                $appID = $form['courses'][$i]['application_id']['#default_value'];                                
                                $status = agent_get_application_status($clientID,$appID);
                                ($status == "Not submitted") ? $status = "Draft" : $status = "";
                            }
                            ($status != "Draft") ? $statusdropdown_actv_cls = 'waves-red' : $statusdropdown_actv_cls = '';
                            if (intval($form['courses'][$i]['institution']['#default_value']) > 0) {
                                $uni_detail_arr = osscapplications_get_unilogo_coursename($form['courses'][$i]['institution']['#default_value'], $form['courses'][$i]['course']['#default_value']);
                            } else {
                                $uni_detail_arr = osscapplications_get_unilogo_coursename($form['courses'][$i]['institution_id']['#default_value'], $form['courses'][$i]['course_id']['#default_value']);
                            }
                            if (is_array($uni_detail_arr) && count($uni_detail_arr) > 0) {

                                $uni_title = $uni_detail_arr['uni_title'];
                                $uni_logo = $uni_detail_arr['image_url'];
                                $course_title = $uni_detail_arr['course_title'];
                            } else {

                                $uni_title = '';
                                $uni_logo = '';
                                $course_title = '';
                            }
                            $save = '';
                            $save1 = '';
                            $applylater = '';
                            $last_section = '';
                            ?>
                            <li class="test_class">
                                <?php if ($j == ($num_courses - 1)) { ?>
                                    <div class="collapsible-header main_add"><span class="profilethumb-logo"><i class="fa fa-question-circle" style="font-size: 29px;"></i>
                                        </span></div>
                                    <?php
                                    $save = drupal_render($form['courses'][$i]['save']);
                                    $save1 = drupal_render($form['courses'][$i]['savenew']);
                                    $applylater = drupal_render($form['courses'][$i]['applylater']);
                                    $last_section = '<div class="formcon overflow-hide lastoverflow">';
                                } else {
                                    $save = drupal_render($form['courses'][$i]['save']);
                                    $save1 = drupal_render($form['courses'][$i]['savenew']);
                                    $applylater = drupal_render($form['courses'][$i]['applylater']);
                                    ?>
                                    <div class="collapsible-header app-section"><span class="profilethumb-logo"><img src="<?php print $uni_logo; ?>" width="20" height="29" alt="logo1"></span> <span class="profile-uniname"><?php print $course_title; ?></span>
                                        <span class="date">
                                            <span class="status-drop"><?php print $status; ?></span><?php print render($form['courses'][$i]['drop_status']); ?>
                                            <a class="waves-effect waves-light <?php print $statusdropdown_actv_cls; ?> btn dropdown-button righttopdrop" href="#!" id="righttopdrop" data-activates="dropdown<?php print $i; ?>" style="background:none; padding:0; margin:0; box-shadow: none;"><span class="date-icon"><i class="material-icons md-36 ng-binding" style="margin-right:0; color:#ce0000; line-height: normal;">more_vert</i></span></a>
                                        </span>
                                        <?php print render($form['courses'][$i]['ukuni_application_id']); ?></div>                                
                                            <?php $last_section = '<div class="formcon overflow-hide lastoverflow">';
                                                }
                                            ?>
                                    <div class="collapsible-body"><?php print $last_section; ?>
                                        <div class="stop-slide-drag"><div class="width50">
                                                <div class="input-field col s6 m6">
                                                    <?php
                                                    print render($form['courses'][$i]['institution']);
                                                    print render($form['courses'][$i]['institution_id']);
                                                    print render($form['courses'][$i]['course_id']);
                                                    print render($form['courses'][$i]['application_id']);
                                                    ?>
                                                </div>
                                            </div>
                                            <div id="cour-drop" class="width50 mrnone">
                                                <div class="input-field col s6 m6">
                                                    <?php print render($form['courses'][$i]['course']); ?>
                                                </div>
                                            </div>
                                            <div class="width50">
                                                <div class="input-field col s6 m6">
                                                    <?php print render($form['courses'][$i]['intake']); ?>
                                                </div>
                                            </div>
                                            <div class="width50 mrnone">
                                                <div class="input-field col s6 m6">
                                                    <?php print render($form['courses'][$i]['entry_year']); ?>
                                                </div>
                                            </div>
                                            <div class="width50">
                                                <div class="input-field col s6 m6">
                                                    <?php print render($form['courses'][$i]['pre_sessional_english']); ?>
                                                </div>
                                            </div>                   
                                            <?php 
                                           //print_r($form['courses'][$i]['client']['#default']);
                                            //exit;
                                            ?>
                                            <div class="width50 mrnone">
                                                <div class="input-field col s6 m6">
                                                    <?php
                                                        print render($form['courses'][$i]['status']);
                                                        print render($form['courses'][$i]['client']);                                                     
                                                        print render($form['courses'][$i]['newcourse']);
                                                        print render($form['courses'][$i]['NewAppIndex']);
                                                    ?>
                                                </div>
                                            </div>                                                                                                                                                                                
                                        </div>
                                        <div class="clearfix"></div>
                                        <?php
                                        print $save;
                                        print $save1;
                                        print $applylater;
                                        print render($form['courses'][$i]['cancel']);                                        
                                        ?>
                                    </div>
                                </div>
                            </li>
                            <?php
                            $j++;
                        }
                        ?></ul>
                </div>
            </div>       
            </div>
        </section>
    </div>
</div><!-- #main -->


