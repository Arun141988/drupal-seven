<?php
global $user;
$form = $variables['form'];
$output = '';
$theme_path = drupal_get_path('theme', 'bootstrap');
print render($form['form_id']);
print render($form['form_build_id']);
print render($form['form_token']);
$clientID = $form['account']['clientID']['#default_value'];
$cls_arry = agent_setactive_or_inactive($clientID);
?>
<div id="main">           
    <div id="content">                
        <section class="profileforminn">
            <div class="profileslide">    
                <div class="profilemidinn">                
                    <ul class="collapsible popout collapsible-accordion" data-collapsible="accordion">
                        <li>
                            <div class="collapsible-header <?php print $cls_arry['acc_class']; ?>"><i class="material-icons">account_circle</i>Account<span style="float:right;"><?php print $cls_arry['acc']; ?></span></div>
                            <div class="collapsible-body">
                                <div class="formcon overflow-hide">
                                    <div class="mob-form-div">
                                        <div class="stop-slide-drag">
                                            <div class="width50 mrnone">
                                                <?php print render($form['account']['clientID']); ?> 
                                                <?php print render($form['account']['email']); ?>
                                            </div>
                                            <div class="width50"><?php print render($form['account']['telephone']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['account']['promo_code']); ?></div>
                                            <?php if(intval($clientID)==0) { ?>
                                                    <div class="width50"><?php print render($form['account']['emailnotifications']); ?></div>
                                            <?php } ?>
                                        </div>
                                        <div class="clearfix"></div>
                                        <?php print render($form['save1']); ?>
                                        <?php print render($form['cancel']); ?>
                                    </div></div>
                        </li>
                        <li><div class="collapsible-header <?php print $cls_arry['bsc_class']; ?>"><i class="material-icons">list</i>Basic<span style="float:right;"><?php print $cls_arry['bsc']; ?></span></div>
                            <div class="collapsible-body"><div class="formcon overflow-hide">
                                    <div class="mob-form-div">
                                        <div class="stop-slide-drag">
                                            <div class="width50"><?php print render($form['basic']['clientID']); ?><?php print render($form['basic']['first_name']); ?></div>
                                            <div class="width50 mrnone"><?php print drupal_render($form['basic']['surname']); ?></div>
                                            <div class="width50"><?php print render($form['basic']['birthdate']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['basic']['gender']); ?></div>
                                            <div class="width50"><?php print render($form['basic']['nationality']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['basic']['address']); ?></div>
                                            <div class="width50"><?php print render($form['basic']['district']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['basic']['city']); ?></div>
                                            <div class="width50"><?php print render($form['basic']['zipcode']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['basic']['country']); ?></div>
                                            <div class="width50"><?php print render($form['basic']['passport_number']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['basic']['passport_expdate']); ?></div>
                                        </div>
                                    </div>
                                    <div class="clearfix"></div>
                                    <?php print render($form['basic']['step1']); ?>
                                    <?php print render($form['save2']); ?>
                                    <?php print render($form['cancel']); ?>
                                </div>
                            </div>
                        </li>
                        <li>
                            <div class="collapsible-header <?php print $cls_arry['acad_class']; ?>"><i class="material-icons md-36 ng-binding">school</i>Academic<span style="float:right;"><?php print $cls_arry['acad']; ?></span></div>
                            <div class="collapsible-body"><div class="formcon overflow-hide">
                                    <div class="mob-form-div">
                                        <div class="stop-slide-drag">
                                            <div class="width50"><?php print render($form['acadimic']['clientID']); ?><?php print render($form['acadimic']['current_institution']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['acadimic']['current_course']); ?></div>
                                            <div class="width50"><?php print render($form['acadimic']['current_course_level']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['acadimic']['gpa_type']); ?></div>
                                            <div class="width50"><?php print render($form['acadimic']['gpa_score']); ?></div>
                                        </div>
                                    </div>
                                    <div class="clearfix"></div>
                                    <?php print render($form['acadimic']['step2']); ?>
                                    <?php print render($form['save3']); ?>                                
                                    <?php print render($form['cancel']); ?>
                                </div>
                            </div>
                        </li>
                        <li>
                            <div class="collapsible-header <?php $cls_arry['engli_class']; ?> "><i class="material-icons"><img style="padding-left: 2px; -webkit-margin-before: -1em; -webkit-margin-after: -0.3em;" src="/sites/all/themes/bootstrap/images/english-icon.png"></i>English<span style="float:right;"><?php print $cls_arry['engli']; ?></span></div>
                            <div class="collapsible-body">
                                <div class="formcon overflow-hide">
                                    <div class="mob-form-div">
                                        <div class="stop-slide-drag">
                                            <div class="width50"><?php print render($form['english']['clientID']); ?><?php print render($form['english']['test_type']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['english']['test_score']); ?></div>
                                            <div class="width50"><?php print render($form['english']['speaking_test_score']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['english']['writing_test_score']); ?></div>
                                            <div class="width50"><?php print render($form['english']['listening_test_score']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['english']['reading_test_score']); ?></div>
                                        </div>
                                    </div></form>
                                    <?php print render($form['save4']); ?>
                                    <?php print render($form['cancel']); ?>
                                </div>
                            </div>
                        </li>
                        <li>
                            <div class="collapsible-header <?php print $cls_arry['fina_class']; ?>"><i class="material-icons">account_balance_wallet</i>Finance<span style="float:right;"><?php print $cls_arry['fina']; ?></span></div>
                            <div class="collapsible-body">
                                <div class="formcon overflow-hide">
                                    <div class="mob-form-div">
                                        <div class="stop-slide-drag">
                                            <div class="width50"><?php print render($form['finance']['clientID']); ?><?php print render($form['finance']['preferred_currency']); ?></div>
                                            <div class="width50 mrnone"><?php print render($form['finance']['fees_budget']); ?></div>
                                            <div class="width50"><?php print render($form['finance']['scholarship']); ?></div>
                                        </div>
                                    </div>
                                    <div class="clearfix"></div>
                                    <?php print render($form['save5']); ?>
                                    <?php print render($form['cancel']); ?>
                                </div>
                            </div>
                        </li>
                    </ul>
                </div>                                                                                                              
            </div>            
        </section>
    </div>
</div><!-- #main -->
</div>