<?php global $base_url; ?>
<div class="uni-detail-mid-white doc-main-visible">
    <div class="tablegrid">
        <div class="addtable-first-row">
            <div class="addtable-first-col-th"><span class="addtable-td-th-span">Name</span></div>
            <div class="addtable-secound-th"><span class="addtable-td-th-span">Other Name</span></div>
            <div class="addtable-third-th"><span class="ml0 addtable-td-th-span">Basic Status</span></div>
            <div class="addtable-fourth-th"><span class="addtable-td-th-span">Email</span></div>
            <div class="addtable-fifth-th"><span class="ml0 addtable-td-th-span">Telephone</span></div>            
            <div class="addtable-sixth-th"><span class="addtable-td-th-span">Current Institution</span></div>            
            <div class="addtable-seventh-th"><span class="addtable-td-th-span">Date Modified</span></div>            
            <div class="addtable-eight-th"><span class="addtable-td-th-span">Action</span></div>
        </div>
<?php
$gender = array(t('Male'), t('Female'));
foreach ($clients as $row):
    ?>       
        <div class="addtable-row-td">
            <div class="addtable-first-col-td"><span class="addtable-td-th-span"><?php $name=$row->firstname.' '.$row->surname;?><a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $name;?>"><?php echo showTrancateString($name,10); ?></a></span></div>
            <div class="addtable-secound-td"><span class="addtable-td-th-span"><?php echo $row->other_name; ?></span></div>
            <div class="addtable-third-td"><span class="ml0 addtable-td-th-span"><?php echo find_agent_client_basic_status_by_key($row->basic_status); ?></span></div>            
            <div class="addtable-fourth-td"><span class="addtable-td-th-span"><a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $row->mail; ?>"><?php echo showTrancateString($row->mail,23); ?></a></span></div>
            <div class="addtable-fifth-td"><span class="ml0 addtable-td-th-span"><?php echo $row->telephone; ?></span></div>            
            <div class="addtable-sixth-td"><span class="addtable-td-th-span"><a class="tooltipped" data-position="top" data-delay="50" data-tooltip="<?php echo $row->current_institution;?>"><?php echo showTrancateString($row->current_institution,30); ?></a></span></div>
            <div class="addtable-seventh-td"><span class="addtable-td-th-span"><?php $st= strtotime($row->date_modified); if($st){echo date('d/m/Y H:i:s',$st);} else{ echo "   ------  ";} ?></span></div> 
            <div class="addtable-eight-td">
                <span class="addtable-td-th-span">
                    <a href="application/detail/<?php echo $row->uid ?>" class="addtable-buttons">Details</a>
                    <?php if (in_array('agent', array_values($user->roles))) { ?>
                      <a href="edit/<?php echo $row->uid ?>/client" class="addtable-buttons">Edit</a>
                    <?php } ?>
                </span>
            </div>
        </div>
<?php endforeach; ?>        
<?php if(count($clients)==1){?>
        <!--div class="addtable-row-td " style="text-align: center;font-weight: bold;">Client Record Not found</div-->
<?php }?>
</div>     
</div>
<?php echo $pager; ?>


