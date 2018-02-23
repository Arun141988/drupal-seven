<?php
    global $base_url;
    $applications = $applications['applications'];                
    $pre_sessional_englishs = array('0' => t('No'), '1' => t('Yes'),);
?>
<div class="uni-detail-mid-white doc-main-visible">
    <div class="clienttable tablegrid">    
        <div class="addtable-first-row">
            <div class="addtable-first-col-th"><span class="addtable-td-th-span">Client Name</span></div>
            <div class="addtable-secound-th"><span class="addtable-td-th-span">University</span></div>
            <div class="addtable-third-th"><span class="addtable-td-th-span">Course</span></div>
            <div class="addtable-fourth-th"><span class="addtable-td-th-span">Intake</span></div>
            <div class="addtable-seventh-th"><span class="addtable-td-th-span">Status</span></div>
            <div class="addtable-eight-th"><span class="addtable-td-th-span">Date Modified</span></div>
        </div>
<?php if (is_array($applications) && count($applications) > 0) {
      foreach ($applications as $key => $val) {
          $appEditUrl='#';
          if (in_array('agent', array_values($user->roles))) {
            $appEditUrl=$base_url.'/agent/client/add/'.$val['uid'].'/application';
          }
          ?>
        <div class="addtable-row-td">
            <div class="addtable-first-col-td"><span class="addtable-td-th-span"><?php $name=$val['name'].' '.$val['surname']; ?><a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $name;?>" href="<?php echo $appEditUrl;?>"><?php echo showTrancateString($name,15); ?></a></span></div>
            <div class="addtable-secound-td"><span class="addtable-td-th-span"><a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $val['u_title'];?>"><?php echo showTrancateString($val['u_title'],25); ?></a></span></div>
            <div class="addtable-third-td"><span class="addtable-td-th-span"> <a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $val['c_title'];?>"><?php echo showTrancateString($val['c_title'],30); ?></a></span></div>
            <div class="addtable-fourth-td"><span class="addtable-td-th-span"><?php echo intake_entry_name('intake', $val['intake']); ?></span></div>
            <div class="addtable-seventh-td"><span class="addtable-td-th-span"><a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo showAppStatus($val['status']);?>"><?php echo showTrancateString(showAppStatus($val['status']),25); ?></a></span></div>
            <div class="addtable-eight-td"><span class="addtable-td-th-span"><?php $st= strtotime($val['date_modified']); if($st){echo date('d/m/Y H:i:s',$st);} else{ echo "   ------  ";} ?></span></div>
        </div>
    <?php     
      }    
    } ?>   
    </div>    
</div>
<?php echo $pager;?>