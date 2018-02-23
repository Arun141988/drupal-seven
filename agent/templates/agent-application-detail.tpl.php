<?php
global $base_url; $user;
$prifile  =  $appdetail['prifile'];
($prifile['uid'])? $clientID =  $prifile['uid']:$clientID = 0;
$applications = $appdetail['applications'];
$document = $appdetail['document'];
$gender = array(t(Male), t(Female));
$pre_sessional_englishs = array('0' => t('No'), '1' => t('Yes'),);
?>
<div class="uni-detail-mid-white doc-main-visible">
    <div class="tablegrid">
        <div class="basicinfotablerow-title"><span class="addtable-td-th-span">BASIC INFORMATION</span><span class="addtable-td-th-span back-link"><a href="<?php $base_url; ?>/agent/clients"><i class="fa fa-arrow-left" aria-hidden="true"></i>
 Back To Clients Listing</a></span></div>
        <div class="basicinfotablerow">
            <div class="basicinfotablerow-firstcol"><span class="addtable-td-th-span heading-wt">First Name</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-seccol"><span class="addtable-td-th-span"><?php echo $prifile['firstname']; ?></span></div>
            <div class="basicinfotablerow-thirdcol"><span class="addtable-td-th-span heading-wt">Surname</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-fourthcol"><span class="addtable-td-th-span"><?php echo $prifile['surname']; ?></span></div>
        </div>
        <div class="basicinfotablerow">
            <div class="basicinfotablerow-firstcol"><span class="addtable-td-th-span heading-wt">Date of Birth</span><span class="data-sep"></span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-seccol"><span class="addtable-td-th-span"><?php echo date('d/m/Y', $prifile['birthday']); ?></span></div>
            <div class="basicinfotablerow-thirdcol"><span class="addtable-td-th-span heading-wt">Gender</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-fourthcol"><span class="addtable-td-th-span"><?php echo $gender[$prifile['gender']]; ?></span></div>
        </div>
        <div class="basicinfotablerow">
            <div class="basicinfotablerow-firstcol"><span class="addtable-td-th-span heading-wt">Address</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-seccol"><span class="addtable-td-th-span"><?php echo $prifile['address']; ?></span></div>
            <div class="basicinfotablerow-thirdcol"><span class="addtable-td-th-span heading-wt">City</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-fourthcol"><span class="addtable-td-th-span"><?php echo $prifile['city']; ?></span></div>
        </div>
        <div class="basicinfotablerow">
            <div class="basicinfotablerow-firstcol"><span class="addtable-td-th-span heading-wt">District</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-seccol"><span class="addtable-td-th-span"><?php echo $prifile['district']; ?></span></div>
            <div class="basicinfotablerow-thirdcol"><span class="addtable-td-th-span heading-wt">Postcode</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-fourthcol"><span class="addtable-td-th-span"><?php echo $prifile['zip']; ?></span></div>
        </div>
        <div class="basicinfotablerow">
            <div class="basicinfotablerow-firstcol"><span class="addtable-td-th-span heading-wt">Email</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-seccol"><span class="addtable-td-th-span"><?php echo $prifile['mail']; ?></span></div>
            <div class="basicinfotablerow-thirdcol"><span class="addtable-td-th-span heading-wt">Telephone</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-fourthcol"><span class="addtable-td-th-span"><?php echo $prifile['telephone']; ?></span></div>
        </div>
        <div class="basicinfotablerow">
            <div class="basicinfotablerow-firstcol"><span class="addtable-td-th-span heading-wt">Current Institution</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-seccol"><span class="addtable-td-th-span"><a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $prifile['current_institution'];?>"><?php echo showTrancateString($prifile['current_institution'],30); ?></a></span></div>
            <div class="basicinfotablerow-thirdcol"><span class="addtable-td-th-span heading-wt">Current Course</span><span class="data-sep">:</span></div>
            <div class="basicinfotablerow-fourthcol"><span class="addtable-td-th-span"><?php echo $prifile['current_course']; ?></span></div>
        </div>                
            <div class="basicinfotablerow-title"><span class="addtable-td-th-span">APPLICATIONS</span>
              <?php if (in_array('agent', array_values($user->roles))) { ?>
                <span class="addtable-td-th-span"><a href="<?php echo $base_url; ?>/agent/client/add/<?php echo $clientID; ?>/application" class="waves-effect waves-circle waves-light btn-floating">
                <i class="material-icons">add</i></a></span>
              <?php } ?>
            </div>
        
        
       <?php if (is_array($applications) && count($applications) > 0) { ?>
        <div class="addtable-first-row">
            <div class="basicinfotablerow-firstcol"><span class="addtable-td-th-span"><strong>University</strong></span></div>
            <div class="basicinfotablerow-seccol"><strong>Course</strong></div>
            <div class="basicinfotablerow-thirdcol-app"><strong>Intake</strong></div>
            <div class="basicinfotablerow-fourthcol"><strong>Entry Year</strong></div>
            <div class="basicinfotablerow-fifthcol"><strong>PS English</strong></div>
            <div class="basicinfotablerow-sixthcol"><strong>Status</strong></div>
        </div>
        <?php foreach ($applications as $key => $val) { ?>
        <div class="basicinfotablesecrow">
            <div class="basicinfotablesecrow-firstcol"><span class="addtable-td-th-span"><a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $val['u_title'];?>"><?php echo showTrancateString($val['u_title'],25); ?></a></span></div>
            <div class="basicinfotablesecrow-seccol"><a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $val['c_title'];?>"><?php echo showTrancateString($val['c_title'],25); ?></a></div>
            <div class="basicinfotablesecrow-thirdcol-app"><?php echo intake_entry_name('intake', $val['intake']); ?></div>
            <div class="basicinfotablesecrow-fourthcol"><?php echo intake_entry_name('entry_year', $val['entry_year']); ?></div>
            <div class="basicinfotablesecrow-fifthcol"><?php echo $pre_sessional_englishs[$val['pre_sessional_english']]; ?></div>
            <div class="basicinfotablesecrow-sixthcol"><?php echo showAppStatus($val['status']); ?></div>
        </div>
       <?php }} ?>          
        <div class="basicinfotablerow-title"><span class="addtable-td-th-span">DOCUMENTS</span>
            <?php if (in_array('agent', array_values($user->roles))) { ?>
             <span class="addtable-td-th-span"><a href="<?php echo $base_url; ?>/agent/client/add/<?php echo $clientID; ?>/document" class="waves-effect waves-circle waves-light btn-floating">
             <i class="material-icons">add</i></a></span>
            <?php } ?>
        </div>          
        <?php if (is_array($document) && count($document) > 0) { ?>
        <div class="addtable-first-row">
            <div class="basicinfotablerow-firstcol"><span class="addtable-td-th-span"><strong>File Name</strong></span></div>
            <div class="basicinfotablerow-seccol"><strong>Type</strong></div>
            <div class="basicinfotablerow-sixthcol"><strong>Application Specifics/All Documents</strong></div>            
            <div class="basicinfotablerow-thirdcol"><strong></strong></div>            
        </div>
        <?php foreach ($document as $key => $val) { ?>
        <div class="basicinfotablesecrow">
            <div class="basicinfotablesecrow-firstcol"><span class="addtable-td-th-span"><a href="<?php echo $val['fullpath']; ?>" download class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $val['filename'];?>"><?php echo showTrancateString($val['filename'],25); ?></a></span></div>
            <div class="basicinfotablesecrow-seccol"><?php echo $val['DocumentTypeName']; ?></div>
            <div class="basicinfotablesecrow-sixthcol"><?php echo $val['DocumentType']; ?></div>
            <div class="basicinfotablesecrow-thirdcol"><?php echo $val['SugarCRMAppIA']; ?></div>            
        </div>
        <?php        
         } }          
        ?>        
    </div>
</div>





