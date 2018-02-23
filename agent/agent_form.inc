<?php

/*
 * Implement agent client form.
 */
function agent_client_form( $form, &$form_state ) {
  global $user;
  global $base_root;
  $clientID = $form_state[ 'build_info' ][ 'args' ][ 0 ];
  (intval( $form_state[ 'build_info' ][ 'args' ][ 0 ] ) > 0) ? $clientID = $form_state[ 'build_info' ][ 'args' ][ 0 ] : $clientID = 0;
  check_agent_user_edit_permissions( $clientID );
  checkSubagentAccess();
  $countries = array( '' => t( 'Select' ) );
  $countries += countries_country_get_list();
  $nationalities = array( '' => t( 'Select' ) );
  $nationalities += nationalities_nationality_get_list();
  $info = agent_get_client_basic_info( $clientID );

  /* Profile Tab Account Section form Element Start */
  $form[ '#tree' ] = TRUE;
  $form[ 'account' ][ 'clientID' ] = array(
    '#type' => 'hidden',
    '#default_value' => $clientID,
  );
  $form[ 'account' ][ 'email' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#default_value' => $info[ 'mail' ],
    '#title' => t( 'Email' ),
    '#title_display' => 'profileform',
    '#required' => TRUE,
  );
  if ( $info[ 'mail' ] != '' ) {
    $form[ 'account' ][ 'email' ][ '#disabled' ] = TRUE;
  }
  else {
    $form[ 'account' ][ 'email' ][ '#disabled' ] = FALSE;
  }
  $form[ 'account' ][ 'telephone' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Mobile' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'telephone' ],
    '#required' => TRUE,
  );

  $form[ 'account' ][ 'promo_code' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'UKuni Promo Code' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'promo_code' ],
    '#required' => FALSE,
  );

  $form[ 'account' ][ 'emailnotifications' ] = array(
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Email Notification' ),
    '#title_display_custom_attribute' => 'select-title',
    '#title_display' => 'profileform',
    '#default_value' => '0',
    '#options' => array(
      '0' => t( 'No' ),
      '1' => t( 'Yes' ),
    ),
  );

  $form[ 'save1' ] = array(
    '#type' => 'submit',
    '#value' => 'Save<span>1</span>',
    '#limit_validation_errors' => array( array( 'account' ) ),
    '#attributes' => array( 'class' => array( 'waves-effect', 'waves-light', 'btn', 'btn-primary' ) ),
    '#submit' => array( 'agent_client_form_submit1' ),
    '#validate' => array( 'agent_client_form_validate1' ),
  );
  /* Profile Tab Account Section form Element End */

  /* Profile Tab Basic Section form Element Start */
  $form[ 'basic' ][ 'clientID' ] = array(
    '#type' => 'hidden',
    '#default_value' => $clientID,
  );
  $form[ 'basic' ][ 'first_name' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Passport First Name' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'firstname' ],
    '#required' => TRUE,
  );
  $form[ 'basic' ][ 'step1' ] = array(
    '#type' => 'hidden',
    '#title' => t( 'Basic section completed or not status' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'step1' ],
  );
  $form[ 'basic' ][ 'surname' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Passport Last Name' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'surname' ],
    '#required' => TRUE,
  );
  $form[ 'basic' ][ 'birthdate' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Birthdate' ),
    '#title_display' => 'profileform',
    '#attributes' => array(
      'class' => array( 'datepicker' ),
    ),
    '#default_value' => $info[ 'birthday' ],
    '#required' => TRUE,
  );
  $form[ 'basic' ][ 'gender' ] = array(
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',  
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'gender' ],    
    '#options' => array(
      '' => t( 'Gender' ),
      '0' => t( 'Male' ),
      '1' => t( 'Female' ),
    ),
  );
  
  ($info[ 'nationality' ] == '') ? $default_nationality = 'Nationality' : $default_nationality = $info[ 'nationality' ];
  $form[ 'basic' ][ 'nationality' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6 rightarrow"><span class="caret">▼</span>',
    '#suffix' => '</div>',
    '#title' => t( 'Nationality' ),
    '#title_display' => 'profileform',
    '#attributes' => array(
      'class' => array( 'nationality_dropdown' ),
    ),
    '#required' => TRUE,
    '#default_value' => fetchTitleByCodeForNationalities( $default_nationality ),
  );

  $form[ 'basic' ][ 'address' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Address' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'address' ],
    '#required' => TRUE,
  );

  $form[ 'basic' ][ 'district' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'District' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'district' ],
    '#required' => TRUE,
  );

  $form[ 'basic' ][ 'city' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'City' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'city' ],
    '#required' => TRUE,
  );

  $form[ 'basic' ][ 'zipcode' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Postcode' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'zip' ],   
  );

  ($info[ 'country' ]) ? $default_country = $info[ 'country' ] : $default_country = 'Country';
  $form[ 'basic' ][ 'country' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6 rightarrow"><span class="caret">▼</span>',
    '#suffix' => '</div>',
    '#title' => t( 'Country' ),
    '#title_display' => 'profileform',
    '#attributes' => array( 'class' => array( 'countries_dropdown' ),
    ),
    '#default_value' => fetchTitleByCodeForCountry( $default_country ),
    '#required' => TRUE,
  );

  $form[ 'basic' ][ 'passport_number' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Passport Number' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'passport_number' ],   
  );

  $form[ 'basic' ][ 'passport_expdate' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Passport Expiry Date' ),
    '#title_display' => 'profileform',
    '#attributes' => array(
      'class' => array( 'passportdatepicker' ),
    ),
    '#default_value' => $info[ 'passport_expdate' ],   
  );
  $form[ 'save2' ] = array(
    '#type' => 'submit',
    '#value' => 'Save<span>2</span>',
    '#attributes' => array( 'class' => array( 'waves-effect', 'waves-light', 'btn', 'btn-primary' ) ),
    '#limit_validation_errors' => array( array( 'account' ), array( 'basic' ) ),
    '#submit' => array( 'agent_client_form_submit2' ),
    '#validate' => array( 'agent_client_form_validate2' ),
  );
  /* Profile Tab Basic Section form Element End */

  /* Profile Tab acadimic Section form Element start */
  $form[ 'acadimic' ][ 'clientID' ] = array(
    '#type' => 'hidden',
    '#default_value' => $clientID,
  );
  $form[ 'acadimic' ][ 'current_institution' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Current Institution' ),
    '#title_display' => 'profileform',
    '#attributes' => array( 'class' => array( 'ossapplication-current-institution' ) ),
    '#default_value' => $info[ 'current_institution' ],
    '#required' => TRUE,
  );

  $form[ 'acadimic' ][ 'current_course' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Current Course' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'current_course' ],
    '#required' => TRUE,
  );

  ($info[ 'current_course_level' ]) ? $default_course_level = $info[ 'current_course_level' ] : $default_course_level = '';
  $form[ 'acadimic' ][ 'current_course_level' ] = array(
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Current Course Level' ),
    '#title_display_custom_attribute' => 'select-title',
    '#title_display' => 'profileform',
    //'#disabled_options' => array('-'),
    '#required' => TRUE,
    '#default_value' => $default_course_level,
    '#options' => array(
      '' => t( 'Select Current Course Level' ),
      '1' => t( 'High School' ),
      '2' => t( 'Undergraduate' ),
      '3' => t( 'Postgraduate' ),
      '4' => t( 'Foundation' ),
      '5' => t( 'Pre-Masters' ),
      '6' => t( 'English Language' ),
      '7' => t( 'Diploma, First Year Degree, International Year One' ),
      '8' => t( 'Junior School' ),
    ),
  );

  ($info[ 'gpa_type' ] != '') ? $default_gpa_type = $info[ 'gpa_type' ] : $default_gpa_type = 'GPA Type';
  $form[ 'acadimic' ][ 'gpa_type' ] = array(
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'GPA Type' ),
    '#title_display' => 'profileform',
    '#default_value' => $default_gpa_type,    
    '#options' => array(
      '' => t( 'GPA Type' ),
      '0' => t( '4 Point Scale' ),
      '1' => t( '5 Point Scale' ),
      '2' => t( 'Percentage' ),
    ),
  );

  $form[ 'acadimic' ][ 'gpa_score' ] = array(
    '#type' => 'textfield',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'GPA Score' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'gpa_score' ],
    '#required' => FALSE,
  );

  $form[ 'acadimic' ][ 'step2' ] = array(
    '#type' => 'hidden',
    '#title' => t( 'acadimic section completed or not status' ),
    '#title_display' => 'profileform',
    '#default_value' => $info[ 'step2' ],
  );

  $form[ 'save3' ] = array(
    '#type' => 'submit',
    '#value' => 'Save<span>3</span>',
    '#limit_validation_errors' => array( array( 'account' ), array( 'basic' ), array( 'acadimic' ) ),
    '#attributes' => array( 'class' => array( 'waves-effect', 'waves-light', 'btn', 'btn-primary' ) ),
    '#submit' => array( 'agent_client_form_submit3' ),
    '#validate' => array( 'agent_client_form_validate3' ),
  );
  /* Profile Tab Acadimic Section Form Element End */

  /* Profile Tab English Section Form Element start */
  $form[ 'english' ][ 'clientID' ] = array(
    '#type' => 'hidden',
    '#default_value' => $clientID,
  );
  ($info[ 'test_type' ] == '') ? $default_test_type = 'Test Type' : $default_test_type = $info[ 'test_type' ];
  $form[ 'english' ][ 'test_type' ] = array(
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Test Type' ),
    '#title_display' => 'profileform',
    '#disabled_options' => array( '-' ),
    '#default_value' => $info[ 'test_type' ],
    '#options' => array(
      '' => t( 'Test Type' ),
      '0' => t( 'Not yet taken' ),
      '1' => t( 'IELTS' ),
    ),
  );

  ($info[ 'test_score' ] == '') ? $default_test_score = 'Test Score' : $default_test_score = $info[ 'test_score' ];
  $form[ 'english' ][ 'test_score' ] = array(
    '#states' => array(
      'visible' => array(
        ':input[name="test_type"]' => array( 'value' => '1' ),
      ),
    ),
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Overall Score' ),
    '#title_display' => 'profileform',
    '#disabled_options' => array( '' ),
    '#default_value' => $default_test_score,
    '#options' => array(
      '-' => t( 'Overall Score' ),
      '0' => t( '2.5' ),
      '1' => t( '3.0' ),
      '2' => t( '3.5' ),
      '3' => t( '4.0' ),
      '4' => t( '4.5' ),
      '5' => t( '5.0' ),
      '6' => t( '5.5' ),
      '7' => t( '6.0' ),
      '8' => t( '6.5' ),
      '9' => t( '7.0' ),
      '10' => t( '7.5' ),
      '11' => t( '8.0' ),
      '12' => t( '8.5' ),
      '13' => t( '9.0' ),
    ),
  );
  ($info[ 'speaking_test_score' ] == '') ? $default_speaking = 'Speaking' : $default_speaking = $info[ 'speaking_test_score' ];
  $form[ 'english' ][ 'speaking_test_score' ] = array(
    '#states' => array(
      'visible' => array(
        ':input[name="test_type"]' => array( 'value' => '1' ),
      ),
    ),
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Speaking' ),
    '#title_display' => 'profileform',
    //'#disabled_options' => array('-'),
    '#default_value' => $default_speaking,
    '#options' => array(
      ' ' => t( 'Speaking' ),
      '0' => t( '2.5' ),
      '1' => t( '3.0' ),
      '2' => t( '3.5' ),
      '3' => t( '4.0' ),
      '4' => t( '4.5' ),
      '5' => t( '5.0' ),
      '6' => t( '5.5' ),
      '7' => t( '6.0' ),
      '8' => t( '6.5' ),
      '9' => t( '7.0' ),
      '10' => t( '7.5' ),
      '11' => t( '8.0' ),
      '12' => t( '8.5' ),
      '13' => t( '9.0' ),
    ),
  );

  ($info[ 'writing_test_score' ] == '') ? $default_writing = 'Writing' : $default_writing = $info[ 'writing_test_score' ];
  $form[ 'english' ][ 'writing_test_score' ] = array(
    '#states' => array(
      'visible' => array(
        ':input[name="test_type"]' => array( 'value' => '1' ),
      ),
    ),
    '#type' => 'select',
    '#title' => t( 'Writing' ),
    '#title_display' => 'profileform',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#default_value' => $default_writing,    
    '#options' => array(
      '' => t( 'Writing' ),
      '0' => t( '2.5' ),
      '1' => t( '3.0' ),
      '2' => t( '3.5' ),
      '3' => t( '4.0' ),
      '4' => t( '4.5' ),
      '5' => t( '5.0' ),
      '6' => t( '5.5' ),
      '7' => t( '6.0' ),
      '8' => t( '6.5' ),
      '9' => t( '7.0' ),
      '10' => t( '7.5' ),
      '11' => t( '8.0' ),
      '12' => t( '8.5' ),
      '13' => t( '9.0' ),
    ),
  );

  ($info[ 'listening_test_score' ] == '') ? $default_Listening = 'Listening' : $default_Listening = $info[ 'listening_test_score' ];
  $form[ 'english' ][ 'listening_test_score' ] = array(
    '#states' => array(
      'visible' => array(
        ':input[name="test_type"]' => array( 'value' => '1' ),
      ),
    ),
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#type' => 'select',
    '#title_display' => 'profileform',
    '#title' => t( 'Listening' ),    
    '#default_value' => $default_Listening,
    '#options' => array(
      ' ' => t( 'Listening' ),
      '0' => t( '2.5' ),
      '1' => t( '3.0' ),
      '2' => t( '3.5' ),
      '3' => t( '4.0' ),
      '4' => t( '4.5' ),
      '5' => t( '5.0' ),
      '6' => t( '5.5' ),
      '7' => t( '6.0' ),
      '8' => t( '6.5' ),
      '9' => t( '7.0' ),
      '10' => t( '7.5' ),
      '11' => t( '8.0' ),
      '12' => t( '8.5' ),
      '13' => t( '9.0' ),
    ),
  );

  ($info[ 'reading_test_score' ] == '') ? $default_reading = 'Reading' : $default_reading = $info[ 'reading_test_score' ];
  $form[ 'english' ][ 'reading_test_score' ] = array(
    '#states' => array(
      'visible' => array(
        ':input[name="test_type"]' => array( 'value' => '1' ),
      ),
    ),
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Reading' ),
    '#title_display' => 'profileform',
    '#default_value' => $default_reading,
    '#options' => array(
      '' => t( 'Reading' ),
      '0' => t( '2.5' ),
      '1' => t( '3.0' ),
      '2' => t( '3.5' ),
      '3' => t( '4.0' ),
      '4' => t( '4.5' ),
      '5' => t( '5.0' ),
      '6' => t( '5.5' ),
      '7' => t( '6.0' ),
      '8' => t( '6.5' ),
      '9' => t( '7.0' ),
      '10' => t( '7.5' ),
      '11' => t( '8.0' ),
      '12' => t( '8.5' ),
      '13' => t( '9.0' ),
    ),
  );

  $form[ 'save4' ] = array(
    '#type' => 'submit',
    '#value' => 'Save<span>4</span>',
    '#attributes' => array( 'class' => array( 'waves-effect', 'waves-light', 'btn', 'btn-primary' ) ),
    '#limit_validation_errors' => array( array( 'account' ), array( 'basic' ), array( 'acadimic' ), array( 'english' ) ),
    '#submit' => array( 'agent_client_form_submit4' ),
    '#validate' => array( 'agent_client_form_validate4' ),
  );

  /* Profile Tab Section 5 Start */
  $form[ 'finance' ][ 'clientID' ] = array(
    '#type' => 'hidden',
    '#default_value' => $clientID,
  );
  ($info[ 'preferred_currency' ] == '') ? $default_preferred_currency = 'Preferred Currency' : $default_preferred_currency = $info[ 'preferred_currency' ];
  $form[ 'finance' ][ 'preferred_currency' ] = array(
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Preferred Currency' ),
    '#title_display' => 'profileform',
    '#default_value' => $default_preferred_currency,
    '#options' => array(
      '' => t( 'Preferred Currency' ),
      '1' => t( 'GBP' ),
    ),
  );


  ($info[ 'fees_budget' ] == 0) ? $range_value = '0' : $range_value = $info[ 'fees_budget' ];
  $form[ 'finance' ][ 'fees_budget' ] = array(
    '#type' => 'hidden',
    '#prefix' => '<span class="fees-budget" for="first_name">' . t( 'Fees Budget' ) . ' :&pound; <span id="range_value_fee_display"> ' . $range_value . ' </span></span><div class="input-field col s6 fees-budget-box"><div class="range-field ">
                        <input type="range" min="0" max="50000" step="500" value = "' . $range_value . '" onchange="updateTextInput(this.value);" />',
    '#suffix' => '</div></div>',
    '#attributes' => array( 'id' => array( 'range_value_fee' ) ),
    '#title_display' => 'profileform',
    '#default_value' => $range_value,
    '#required' => FALSE,
  );


  ($info[ 'scholarship' ] == '') ? $default_scholarship = 'Scholarship' : $default_scholarship = $info[ 'scholarship' ];
  $form[ 'finance' ][ 'scholarship' ] = array(
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Scholarship' ),
    '#title_display' => 'profileform',
    '#default_value' => $default_scholarship,
    '#options' => array(
      '' => t( 'Scholarship' ),
      '1' => t( 'Elligible for Government Scholarship' ),
      '2' => t( 'Want Full scholarship' ),
      '3' => t( 'Want Part Scholarship' ),
    ),
  );


  $form[ 'save5' ] = array(
    '#type' => 'submit',
    '#value' => 'Save<span>5</span>',
    '#attributes' => array( 'class' => array( 'waves-effect', 'waves-light', 'btn', 'btn-primary' ) ),
    '#limit_validation_errors' => array( array( 'account' ), array( 'basic' ), array( 'acadimic' ), array( 'english' ), array( 'finance' ) ),
    '#submit' => array( 'agent_client_form_submit5' ),
    '#validate' => array( 'agent_client_form_validate5' ),
  );
  $form[ 'cancel' ] = array(
    '#type' => 'submit',
    '#value' => t( 'Cancel' ),
    '#access' => TRUE,
    '#weight' => 15,
    '#attributes' => array( 'class' => array( 'btn', 'btn-primary' ) ),
    '#submit' => array( 'agent_client_form_cancel' ),
    '#limit_validation_errors' => array(),
  );
  /* Profile Tab Section 5 End */
  return $form;
}

/*
 * Implement add_application_form().
 */
function add_application_form( $form, &$form_state ) {
  checkSubagentAccess();  
  global $base_root;
  /* Below code line user for get user id from url */
  $ID = $form_state[ 'build_info' ][ 'args' ][ 0 ];
  (intval( $ID ) > 0) ? $clientID = $ID : $clientID = 0;
  $agentID = $user->uid;
  check_agent_user_edit_permissions( $clientID );  
  $form[ '#tree' ] = TRUE;
  if ( empty( $form_state[ 'num_courses_new' ] ) ) {
    $form_state[ 'num_courses_new' ] = 1;
  }
  // Get courses from {application_step2}
  $query = db_select( 'application_step2', 's' );
  $query->condition( 's.uid', $clientID, '=' );
  $def_app_or = db_or()->condition( 's.status', 'NULL', 'IS NULL' )->condition( 's.status', 'Not submitted', '!=' );
  $query->condition( $def_app_or );
  $query->fields( 's', array( 'unid', 'cnid', 'personal_statement', 'reference', 'intake', 'entry_year', 'pre_sessional_english', 'id', 'status', 'sugar_application_identifier' ) );
  $result = $query->execute();

  $courses_record = array();
  $institutions_record = array();
  $specifics = array();
  foreach ( $result as $record ) {
    $institution = $record->unid;
    $course = $record->cnid;
    $intake = $record->intake;
    $entry_year = $record->entry_year;
    $pre_sessional_english = $record->pre_sessional_english;
    $status = $record->status;
    $id = $record->id;
    $courses_record[] = array( 'institution' => $institution, 'course' => $course, 'intake' => $intake, 'entry_year' => $entry_year, 'pre_sessional_english' => $pre_sessional_english, 'status' => $status, 'ukuni_application_id' => $id, );
    $institutions_record[] = $institution;
    $specifics[] = array(
      'unid' => $record->unid,
      'cnid' => $record->cnid,
      'ukuni_application_id' => $record->id,
      'sugar_application_identifier' => $record->sugar_application_identifier,
      'personal_statement' => $record->personal_statement,
      'reference' => $record->reference,
    );
  }

  $favs = '';
  $favs_new = array();
  foreach ( $favs as $id => $fav ) {
    if ( in_array( $fav, $institutions_record ) ) {
      unset( $favs[ $id ] );
    }
    else {
      $favs_new[] = $favs[ $id ];
    }
  }
  $favs = $favs_new;

  $entry_years = array();
  $intakes = array();
  $pre_sessional_englishs = array();

  // Populate Uni name from {application_step2}

  $matches = array();
  $matches = application_institution_list();
  $num_courses_record = count( $courses_record );
  $form[ 'clientID' ] = array(
    '#type' => 'hidden',
    '#default_value' => $clientID,
    '#attributes' => array( 'id' => array( 'client-id' ) ),
  );
  for ( $i = 0; $i < $num_courses_record; $i++ ) {
    $default_value = 'Enter University';
    if ( !empty( $courses_record[ $i ] ) ) {
      $nid_uni = $courses_record[ $i ][ 'institution' ];
      $node_uni = node_load( $nid_uni );
      if ( !empty( $node_uni ) )
        $default_value = $nid_uni;
    }
    $form[ 'courses' ][ $i ][ 'institution' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Institutions' ),
      '#title_display' => 'profileform',
      '#attributes' => array( 'class' => array( 'institutions', 'inst-select' ) ),
      '#options' => $matches,
      '#default_value' => $default_value,
      '#disabled' => TRUE,
      '#ajax' => array(
        'callback' => 'application_insitution_ajax',
        'wrapper' => 'courses',
      ),
    );
    $default_value = NULL;
    $ukuni_application_id = $courses_record[ $i ][ 'ukuni_application_id' ];
    if ( !empty( $ukuni_application_id ) ) {
      $default_value = $ukuni_application_id;
    }
    $form[ 'courses' ][ $i ][ 'ukuni_application_id' ] = array(
      '#type' => 'hidden',
      '#title' => t( 'Ukuni application id' ),
      '#title_display' => 'profileform',
      '#default_value' => $default_value,
      '#disabled' => TRUE,
    );
    // Populate Uni courses from {application_step2}
    $default_value = NULL;
    if ( !empty( $courses_record[ $i ] ) ) {
      $default_value = $courses_record[ $i ][ 'course' ];
      $nid_uni = $courses_record[ $i ][ 'institution' ];
      $courses = application_get_courses_by_nid( $nid_uni );
    }
    if ( !empty( $form_state[ 'values' ][ 'courses' ][ $i ][ 'institution' ] ) ) {
      $title_uni = $form_state[ 'values' ][ 'courses' ][ $i ][ 'institution' ];
      $nid_uni = $courses_record[ $i ][ 'institution' ];
      $courses = application_get_courses_by_nid( $nid_uni );
    }
    $form[ 'courses' ][ $i ][ 'course' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Courses' ),
      '#title_display' => 'profileform',
      '#options' => $courses,
      '#attributes' => array( 'class' => array( 'courses' ) ),
      '#default_value' => $default_value,
      '#disabled' => TRUE,
    );

    // CBA TODO: Make $intakes date options programmatic.
    $current_year = date( "Y" );
    $current_month = date( "m" );
    $intakes = ShowIntakeDropdownForApplications( 1 );
    $default_value = NULL;
    if ( !empty( $courses_record[ $i ] ) ) {
      $default_value = $courses_record[ $i ][ 'intake' ];
    }

    $form[ 'courses' ][ $i ][ 'intake' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Intake' ),
      '#title_display' => 'profileform',
      '#options' => $intakes,
      '#attributes' => array( 'class' => array( 'intake' ) ),
      '#default_value' => $default_value,
      '#disabled' => TRUE,
    );

    $entry_years = array(
      '' => t( 'Select' ),
      '0' => t( 'Year 1' ),
      '1' => t( 'Year 2' ),
      '2' => t( 'Year 3' ),
    );

    $default_value = NULL;
    if ( !empty( $courses_record[ $i ] ) ) {
      $default_value = $courses_record[ $i ][ 'entry_year' ];
    }

    $form[ 'courses' ][ $i ][ 'entry_year' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Entry Year' ),
      '#title_display' => 'profileform',
      '#options' => $entry_years,
      '#attributes' => array( 'class' => array( 'entry-year' ) ),
      '#default_value' => $default_value,
      '#disabled' => TRUE,
    );

    $pre_sessional_englishs = array(
      '' => t( 'Select' ),
      '0' => t( 'No' ),
      '1' => t( 'Yes' ),
    );
    $ukuni_status = array(
      '' => t( 'Select' ),
      'Not submitted' => t( 'Draft' ),
      'Incomplete Application' => t( 'Awaiting Confirmation' ),
      'Application Complete' => t( 'Confirmed' ),
      'Awaiting Agency Confirmation' => t( 'Submitting' ),
      'New Application' => t( 'Submitted' ),
      'Unconditional Offer' => t( 'Unconditional Offer' ),
      'Conditional Offer' => t( 'Conditional Offer' ),
      'offer_accepted' => t( 'Offer Accepted' ),
      'Visa Letter Sent' => t( 'CAS Received' ),
      'Pathway CAS' => t( 'Pathway CAS' ),
      'Visa Letter Withdrawn' => t( 'CAS Withdrawn' ),
      'Student Rejected' => t( 'Rejected' ),
      'Application Cancelled' => t( 'Cancelled' ),
      'Application Resubmitted' => t( 'Resubmitted' ),
      'Finance Verified' => t( 'Finance Verified' ),
      'Partial Deposit Payment' => t( 'Partial Deposit Payment' ),
      'Student Deposit Paid' => t( 'Student Deposit Paid' ),
      'Student Enrolled' => t( 'Student Enrolled' ),
      'Student Reported' => t( 'Student Reported' ),
      'Did Not Enrol' => t( 'Did Not Enrol' ),
      'Visa Accepted' => t( 'Visa Accepted' ),
      'Visa Letter Reissued' => t( 'Visa Letter Reissued' ),
      'Agency Rejected' => t( 'Agency Rejected' ),
      'Visa Rejected' => t( 'Visa Rejected' ),
    );

    $default_value = NULL;
    if ( !empty( $courses_record[ $i ] ) ) {
      $default_value = $courses_record[ $i ][ 'pre_sessional_english' ];
    }

    $form[ 'courses' ][ $i ][ 'pre_sessional_english' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Pre-Sessional English' ),
      '#title_display' => 'profileform',
      '#options' => $pre_sessional_englishs,
      '#attributes' => array( 'class' => array( 'pre-sessional-english' ) ),
      '#default_value' => $default_value,
      '#disabled' => TRUE,
    );

    $default_value = NULL;
    if ( !empty( $courses_record[ $i ] ) ) {
      $default_value = $courses_record[ $i ][ 'status' ];
    }
    $form[ 'courses' ][ $i ][ 'status' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Ukuni Status' ),
      '#title_display' => 'profileform',
      '#options' => $ukuni_status,
      '#attributes' => array( 'class' => array( 'pre-sessional-english' ) ),
      '#default_value' => $default_value,
      '#disabled' => TRUE,
    );

    $form[ 'courses' ][ $i ][ 'remove' ] = array(
      '#type' => 'submit',
      '#title' => t( 'Remove' ),
      '#value' => 'Remove',
      '#attributes' => array( 'class' => array( 'remove' ) ),
      '#name' => 'remove-course-' . $i,
      '#submit' => array( 'application_insitution_remove_submit' ),
      '#ajax' => array(
        'callback' => 'application_insitution_remove_ajax',
        'wrapper' => 'courses',
      ),
    );

    /* new code */
    $default_value = NULL;
    if ( !empty( $courses_record[ $i ] ) ) {
      $default_value = $courses_record[ $i ][ 'status' ];
    }
    $drop_status = status_dropdown( $default_value );
    $liHtml = '';
    foreach ( $drop_status as $key => $val ) {
      if ( $courses_record[ $i ][ 'status' ] == $key ) {
        $test[] = $key;
        $liHtml.='<li id="' . $key . '"><a href="javascript:void(0);" style="color:#cb0000!important;" >' . $val . '</a></li>';
      }
      else {
        $liHtml.='<li id="' . $key . '"><a href="javascript:void(0);" >' . $val . '</a></li>';
      }
    }
    $form[ 'courses' ][ $i ][ 'drop_status' ] = array(
      '#markup' => '<ul id="dropdown' . $i . '" class="dropdown-content dropdown3" >' . $liHtml . '
                                </ul>',
    );
    $liHtml = '';
    $ukuni_application_id = $courses_record[ $i ][ 'ukuni_application_id' ];
    $form[ 'courses' ][ $i ][ 'ukuni_application_id' ] = array(
      '#type' => 'hidden',
      '#title' => t( 'Ukuni application id' ),
      '#title_display' => 'profileform',
      '#default_value' => $ukuni_application_id,
      '#attributes' => array(
        'class' => array( 'ukuni_application_id' ),
      ),
      '#disabled' => TRUE,
    );
  }

  // Populate favourites
  $query = db_select( 'application_step2', 's' );
  $query->condition( 's.uid', $clientID, '=' );
  $query->condition( 's.status', 'Not submitted', '=' );
  $query->fields( 's', array( 'unid', 'cnid', 'personal_statement', 'reference', 'intake', 'entry_year', 'pre_sessional_english', 'id', 'status' ) );
  $SaveAsDraft_result = $query->execute();
  foreach ( $SaveAsDraft_result as $record ) {
    $institution = $record->unid;
    $course = $record->cnid;
    $intake = $record->intake;
    $entry_year = $record->entry_year;
    $pre_sessional_english = $record->pre_sessional_english;
    $status = $record->status;
    $id = $record->id;
    $saveasdraft_record[] = array( 'institution' => $institution, 'course' => $course, 'intake' => $intake, 'entry_year' => $entry_year, 'pre_sessional_english' => $pre_sessional_english, 'status' => $status, 'id' => $id, );
  }

  $num_favs = count( $saveasdraft_record );
  for ( $i = 0; $i < $num_favs; $i++ ) {
    if ( is_array( $saveasdraft_record ) && count( $saveasdraft_record ) > 0 ) {
      $uni_nid = $saveasdraft_record[ $i ][ 'institution' ];
      $uni_title = application_get_title_by_nid( $uni_nid );

      $draftApp_id = $saveasdraft_record[ $i ][ 'id' ];
      $draftApp_course = $saveasdraft_record[ $i ][ 'course' ];
      $draftApp_intake = $saveasdraft_record[ $i ][ 'intake' ];
      $draftApp_entry_year = $saveasdraft_record[ $i ][ 'entry_year' ];
      $draftApp_pre_sessional_english = $saveasdraft_record[ $i ][ 'pre_sessional_english' ];
      $draftApp_status = $saveasdraft_record[ $i ][ 'status' ];
      $universityandcourse = applications_get_unilogo_coursename( $uni_nid, $draftApp_course );
      $course_title = $universityandcourse[ 'course_title' ];
    }

    if ( $uni_title ) {
      $institutions = $uni_title;
    }
    else {
      $institutions = 'Select Institutions';
    }

    $appindex = ($i + $num_courses_record);
    $form[ 'courses' ][ ($i + $num_courses_record) ][ 'appindex' ] = array(
      '#type' => 'hidden',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#default_value' => $appindex,
      '#attributes' => array(
        'class' => array( 'app-index' ),
        'id' => 'app-index-' . $appindex,
      ),
    );

    (trim( $draftApp_id ) != '') ? $application_id = $draftApp_id : $application_id = '';
    $form[ 'courses' ][ ($i + $num_courses_record) ][ 'application_id' ] = array(
      '#type' => 'hidden',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#attributes' => array(
        'id' => 'application-id-' . $appindex
      ),
      '#default_value' => $application_id,
    );

    $form[ 'courses' ][ ($i + $num_courses_record) ][ 'institution' ] = array(
      '#type' => 'textfield',
      '#prefix' => '<div class="input-field col s6 rightarrow"><span class="caret">▼</span>',
      '#suffix' => '</div>',      
      '#title_display' => 'profileform',
      '#attributes' => array(
        'class' => array( 'institutions', 'inst-select', 'institutions-draft' ),
      ),
      '#default_value' => $institutions,
      '#validated' => TRUE
    );

    $default_value = NULL;
    $courses = array( t( 'Select Course' ) );
    if ( !empty( $form_state[ 'values' ][ 'courses' ][ ($i + $num_courses_record) ][ 'institution' ] ) ) {
      if ( $uniId != '' ) {
        $uni_nid = $uniId;
      }
      else {
        $uni_nid = $form_state[ 'values' ][ 'courses' ][ ($i + $num_courses_record) ][ 'institution' ];
      }
      $courses = application_get_courses_by_nid( $uni_nid );
    }

    (trim( $course_title ) != '') ? $default_value = $course_title : $default_value = 'Select Course';
    $form[ 'courses' ][ ($i + $num_courses_record) ][ 'course' ] = array(
      '#type' => 'textfield',
      '#prefix' => '<div class="input-field col s6 rightarrow"><span class="caret">▼</span>',
      '#suffix' => '</div>',     
      '#title_display' => 'profileform',
      '#options' => $courses,
      '#attributes' => array( 'class' => array( 'courses', 'courses-draft' ),
      ),
      '#default_value' => $default_value,
      '#validated' => TRUE,
    );

    $form[ 'courses' ][ ($i + $num_courses_record) ][ 'institution_id' ] = array(
      '#type' => 'hidden',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#default_value' => $uni_nid,
      '#attributes' => array(
        'id' => 'institution-' . $appindex,
      ),
    );

    (trim( $draftApp_course ) != '') ? $uni_ncid = $draftApp_course : $uni_ncid = '';
    $form[ 'courses' ][ ($i + $num_courses_record) ][ 'course_id' ] = array(
      '#type' => 'hidden',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#attributes' => array(
        'id' => 'course-' . $appindex,
      ),
      '#default_value' => $uni_ncid,
    );

    // OSS : "My Application" Intakes
    $current_year = date( "Y" );
    $current_month = date( "m" );
    $intakes = ShowIntakeDropdownForApplications();
    (trim( $draftApp_intake ) != '') ? $intake = $draftApp_intake : $intake = 'Select Intakes';
    $form[ 'courses' ][ ($i + $num_courses_record) ][ 'intake' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Intake' ),
      '#title_display' => 'profileform',
      '#options' => $intakes,
      '#attributes' => array( 'class' => array( 'intake' ), 'id' => 'app-intake-' . $appindex ),
      '#default_value' => $intake,
    );

    $entry_years = array(
      ' ' => t( 'Select Entry Years' ),
      '0' => t( 'Year 1' ),
      '1' => t( 'Year 2' ),
      '2' => t( 'Year 3' ),
    );

    (trim( $draftApp_entry_year ) != '') ? $EntryYears = $draftApp_entry_year : $EntryYears = 'Select Entry Years';
    $form[ 'courses' ][ ($i + $num_courses_record) ][ 'entry_year' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Entry Year' ),
      '#title_display' => 'profileform',
      '#options' => $entry_years,
      '#attributes' => array( 'class' => array( 'entry-year' ), 'id' => 'app-entry-year-' . $appindex ),
      '#default_value' => $EntryYears,
    );

    $pre_sessional_englishs = array(
      ' ' => t( 'Select Presessional English' ),
      '0' => t( 'No' ),
      '1' => t( 'Yes' ),
    );
    (trim( $draftApp_pre_sessional_english ) != '') ? $presessionalenglishs = $draftApp_pre_sessional_english : $presessionalenglishs = 'Select Presessional English';
    $form[ 'courses' ][ ($i + $num_courses_record) ][ 'pre_sessional_english' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Pre-Sessional English' ),
      '#title_display' => 'profileform',
      '#options' => $pre_sessional_englishs,
      '#attributes' => array( 'class' => array( 'pre-sessional-english' ), 'id' => 'app-pre-sessional-' . $appindex ),
      '#default_value' => $presessionalenglishs, // CBA TODO: Default value may not be needed here. Test and make sure.            
    );
    $app_num = 0;
    $app_num = ($i + $num_courses_record);
    $form[ 'courses' ][ $app_num ][ 'NewAppIndex' ] = array(
      '#type' => 'hidden',
      '#required' => TRUE,
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#default_value' => $app_num,
      '#attributes' => array( 'class' => array( 'new_course' ) ),
    );

    /* This button use for save application in dreft (only save Drupal) */
    $appindex_en = base64_encode( $appindex );
    $form[ 'courses' ][ $i + $num_courses_record ][ 'applylater' ] = array(
      '#type' => 'submit',
      '#attributes' => array(
        'class' => array( 'waves-effect', 'waves-light', 'btn', 'btn-primary' ),
      ),
      '#value' => 'Apply Later<span>-' . $appindex_en . "</span>",
      '#validate' => array( 'add_application_form_save_draft_validate' ),
      '#submit' => array( 'add_application_form_save_draft_submit' ),
    );

    // Here submit button is used for submit the form by trigger called from popup.
    $form[ 'courses' ][ $i + $num_courses_record ][ 'savenew' ] = array(
      '#type' => 'submit',
      '#attributes' => array(
        'class' => array( 'modal-op', 'but-hidden' ),
      ),
      '#value' => 'savenew-' . $appindex_en,
      '#submit' => array( 'add_application_form_submit' ),
      '#validate' => array( 'add_application_form_validate' ),
    );
    $agent_client = agent_client_list();
    ($clientID > 0) ? $client_value = $clientID : $client_value = 0;
    $form[ 'courses' ][ $i + $num_courses_record ][ 'client' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Client' ),
      '#title_display' => 'profileform',
      '#options' => $agent_client,
      '#required' => TRUE,
      '#default_value' => $client_value,
    );
    if ( $client_value > 0 ) {
      $form[ 'courses' ][ $i + $num_courses_record ][ 'client' ][ '#disabled' ] = TRUE;
    }
    else {
      $form[ 'courses' ][ $i + $num_courses_record ][ 'client' ][ '#disabled' ] = FALSE;
    }
    // Apply button only for displaying/open the confirmation popup
    $form[ 'courses' ][ $i + $num_courses_record ][ 'save' ] = array(
      '#type' => 'button',
      '#attributes' => array(
        'class' => array( 'waves-effect', 'waves-light', 'btn', 'modal-op', 'btn-primary' ),
      ),
      '#value' => 'Apply Now',
    );
  }

  // Populate new courses
  for ( $i = 0; $i < $form_state[ 'num_courses_new' ]; $i++ ) {
    if ( $uniId != '' ) {
      $uni_nid = $uniId;
      $favuni_title = application_get_title_by_nid( $uni_nid );
    }
    else {
      $uni_nid = '';
    }
    if ( $favuni_title ) {
      $institutions = $favuni_title;
    }
    else {
      $institutions = 'Select Institutions';
    }
    $newappindex = ($i + $num_courses_record + $num_favs);
    $form[ 'courses' ][ ($i + $num_courses_record + $num_favs) ][ 'application_id' ] = array(
      '#type' => 'hidden',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#attributes' => array(
        'id' => 'application-id-' . $newappindex
      ),
    );

    $form[ 'courses' ][ ($i + $num_courses_record + $num_favs) ][ 'institution' ] = array(
      '#type' => 'textfield',
      '#prefix' => '<div class="input-field col s6 rightarrow"><span class="caret">▼</span>',
      '#suffix' => '</div>',
      //'#title' => t('Institutions'),
      '#title_display' => 'profileform',
      '#attributes' => array(
        'class' => array( 'institutions', 'inst-select' ),
        'id' => 'uni_dropdown'
      ),
      '#default_value' => $institutions,
      //'#required' => TRUE,
      '#validated' => TRUE
    );


    $default_value = 'Select Course';
    if ( intval( $courId ) > 0 ) {
      $newcoursetitle = applications_get_unilogo_coursename( $uni_nid, $courId );
      $course_title = $newcoursetitle[ 'course_title' ];
      $default_value = $course_title;
    }
    $form[ 'courses' ][ ($i + $num_courses_record + $num_favs) ][ 'course' ] = array(
      '#type' => 'textfield',
      '#prefix' => '<div class="input-field col s6 rightarrow"><span class="caret">▼</span>',
      '#suffix' => '</div>',
      //'#title' => t('Courses'),
      '#title_display' => 'profileform',
      '#options' => $courses,
      '#attributes' => array( 'class' => array( 'courses' ),
        'id' => 'course_dropdown',
      ),
      '#default_value' => $default_value,
      '#validated' => TRUE,
    );

    $form[ 'courses' ][ ($i + $num_courses_record + $num_favs) ][ 'institution_id' ] = array(
      '#type' => 'hidden',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#default_value' => $uni_nid,
      '#attributes' => array(
        'id' => 'institution'
      ),
    );

    $form[ 'courses' ][ ($i + $num_courses_record + $num_favs) ][ 'course_id' ] = array(
      '#type' => 'hidden',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#attributes' => array(
        'id' => 'course'
      ),
      '#default_value' => $courId,
    );

    // OSS : "My Application" Intakes
    $current_year = date( "Y" );
    $current_month = date( "m" );
    $intakes = ShowIntakeDropdownForApplications();
    $intake = 'Select Intakes';
    $form[ 'courses' ][ ($i + $num_courses_record + $num_favs) ][ 'intake' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Intake' ),
      '#title_display' => 'profileform',
      '#options' => $intakes,
      '#attributes' => array( 'class' => array( 'intake' ), 'id' => 'app-intake-' . $newappindex ),
      '#default_value' => $intake,
    );

    $entry_years = array(
      ' ' => t( 'Select Entry Years' ),
      '0' => t( 'Year 1' ),
      '1' => t( 'Year 2' ),
      '2' => t( 'Year 3' ),
    );

    $EntryYears = 'Select Entry Years';
    $form[ 'courses' ][ ($i + $num_courses_record + $num_favs) ][ 'entry_year' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Entry Year' ),
      '#title_display' => 'profileform',
      '#options' => $entry_years,
      '#attributes' => array( 'class' => array( 'entry-year' ), 'id' => 'app-entry-year-' . $newappindex ),
      '#default_value' => $EntryYears,
    );

    $pre_sessional_englishs = array(
      ' ' => t( 'Select Presessional English' ),
      '0' => t( 'No' ),
      '1' => t( 'Yes' ),
    );
    $presessionalenglishs = 'Select Presessional English';
    $form[ 'courses' ][ ($i + $num_courses_record + $num_favs) ][ 'pre_sessional_english' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Pre-Sessional English' ),
      '#title_display' => 'profileform',
      '#options' => $pre_sessional_englishs,
      '#attributes' => array( 'class' => array( 'pre-sessional-english' ), 'id' => 'app-pre-sessional-' . $newappindex ),
      '#default_value' => $presessionalenglishs, // CBA TODO: Default value may not be needed here. Test and make sure.            
    );
    $app_num = 0;
    $app_num = ($i + $num_courses_record + $num_favs);
    $form[ 'courses' ][ $app_num ][ 'NewAppIndex' ] = array(
      '#type' => 'hidden',
      '#required' => TRUE,
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#default_value' => $app_num,
      '#attributes' => array( 'class' => array( 'new_course' ) ),
    );

    $agent_client = agent_client_list();
    ($clientID > 0) ? $client_value = $clientID : $client_value = 0;
    $form[ 'courses' ][ $i + $num_courses_record + $num_favs ][ 'client' ] = array(
      '#type' => 'select',
      '#prefix' => '<div class="input-field col s6">',
      '#suffix' => '</div>',
      '#title' => t( 'Client' ),
      '#title_display' => 'profileform',
      '#options' => $agent_client,
      '#required' => TRUE,
      '#default_value' => $client_value,
    );
    if ( $client_value > 0 ) {
      $form[ 'courses' ][ $i + $num_courses_record + $num_favs ][ 'client' ][ '#disabled' ] = TRUE;
    }
    else {
      $form[ 'courses' ][ $i + $num_courses_record + $num_favs ][ 'client' ][ '#disabled' ] = FALSE;
    }

    /* This button use for save application in dreft (only save Drupal) */
    $form[ 'courses' ][ $i + $num_courses_record + $num_favs ][ 'applylater' ] = array(
      '#type' => 'submit',
      '#attributes' => array(
        'class' => array( 'waves-effect', 'waves-light', 'btn', 'btn-primary' ),
      ),
      '#value' => 'Apply Later<span>-' . base64_encode( $i + $num_courses_record + $num_favs ) . '</span>',
      '#validate' => array( 'add_application_form_save_draft_validate' ),
      '#submit' => array( 'add_application_form_save_draft_submit' ),
    );

    // Here submit button is used for submit the form by trigger called from popup.
    $form[ 'courses' ][ $i + $num_courses_record + $num_favs ][ 'savenew' ] = array(
      '#type' => 'submit',
      '#attributes' => array(
        'class' => array( 'modal-op', 'but-hidden' ),
      ),
      '#value' => 'savenew-' . base64_encode( $i + $num_courses_record + $num_favs ),
      '#submit' => array( 'add_application_form_submit' ),
      '#validate' => array( 'add_application_form_validate' ),
    );
    // Apply button only for displaying/open the confirmation popup
    $form[ 'courses' ][ $i + $num_courses_record + $num_favs ][ 'save' ] = array(
      '#type' => 'button',
      '#attributes' => array(
        'class' => array( 'waves-effect', 'waves-light', 'btn', 'modal-op', 'btn-primary' ),
      ),
      '#value' => 'Apply Now',
    );
    $form[ 'courses' ][ $i + $num_courses_record + $num_favs ][ 'cancel' ] = array(
      '#type' => 'submit',
      '#value' => t( 'Cancel' ),
      '#access' => TRUE,
      '#weight' => 15,
      '#attributes' => array( 'class' => array( 'btn', 'btn-primary' ) ),
      '#submit' => array( 'add_application_form_cancel' ),
      '#limit_validation_errors' => array(),
    );
  }
  return $form;
}

/*
 * Implements funtion to add documents.
 */
function add_document_form( $form, &$form_state ) {
  checkSubagentAccess();
  global $base_root;
  $form[ '#tree' ] = TRUE;
  (intval( $form_state[ 'build_info' ][ 'args' ][ 0 ] ) > 0) ? $clientID = $form_state[ 'build_info' ][ 'args' ][ 0 ] : $clientID = 0;
  check_agent_user_edit_permissions( $clientID );
  $file_data = agent_get_document( 'ALLAPPLICATION', $clientID );
  //step 3
  foreach ( $file_data as $key => $val ) {
    $form[ 'document_delete' ][ $val->fid ] = array(
      '#type' => 'submit',
      '#title' => t( 'Remove' ),
      '#attributes' => array( 'class' => array( 'remove', 'del-doc' ) ),
      '#name' => 'remove-course-' . $val->fid,
      '#limit_validation_errors' => array( array( 'document_delete' ) ),
      '#validate' => array( 'agent_document_file_delete_submit_validate' ),
      '#submit' => array( "agent_document_file_delete_submit" ),
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'agent_document_file_delete_ajax',
        'wrapper' => 'mydocuments-' . $val->fid,
        'parameters' => array( 'fid' => $val->fid, 'clientid' => $clientID ),
      ),
    );
  }

  $document_type = array(
    '' => t( 'Document Type' ),
    'approved_ps' => t( 'Approved PS' ),
    'bank_statement' => t( 'Bank Statement' ),
    'biometrics_letter' => t( 'Biometrics Letter' ),
    'cas_document' => t( 'CAS Document' ),
    'cover_letter' => t( 'Cover Letter' ),    
    'CV' => t( 'CV' ),
    'degree' => t( 'Degree' ),
    'institution_document' => t( 'Institution Document' ),
    'language_cert' => t( 'Language Certificate' ),   
    'other_doc' => t( 'Other' ),
    'passport' => t( 'Passport' ),
    'payment_receipt' => t( 'Payment Receipt' ),
    'police_registration' => t( 'Police Registration' ),
    'reference' => t( 'Reference' ),    
    'status_cert' => t( 'Status Certificate' ),
    'tb_test' => t( 'TB Test' ),
    'transcript' => t( 'Transcript' ),
    'personal_statement' => t( 'Unapproved PS' ),
    'visas' => t( 'Visas' ),
    'visa_application_form' => t( 'Visa Application Form' ),
  );
  $default_value = Null;
  $form[ 'document_type' ] = array(
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6 m6">',
    '#suffix' => '</div>',
    '#title' => t( 'Document Type' ),
    '#title_display' => 'profileform',
    '#options' => $document_type,
    '#default_value' => $default_value,
  );

  $agent_client = agent_client_list();
  ($clientID > 0) ? $client_value = $clientID : $client_value = 0;
  $form[ 'clientid' ] = array(
    '#type' => 'select',
    '#prefix' => '<div class="input-field col s6">',
    '#suffix' => '</div>',
    '#title' => t( 'Client' ),
    '#title_display' => 'profileform',
    '#options' => $agent_client,
    '#required' => TRUE,
    '#default_value' => $client_value,
  );
  if ( $client_value > 0 ) {
    $form[ 'clientid' ][ '#disabled' ] = TRUE;
  }
  else {
    $form[ 'clientid' ][ '#disabled' ] = FALSE;
  }
  $default_value = NULL;
  $field_name = 'field_student_documents';
  $form += _file_upload_element( $field_name, $node = '', $form, $form_state );
  $form[ 'save' ] = array(
    '#type' => 'submit',
    '#value' => 'Save<span></span>',
    '#attributes' => array( 'class' => array( 'waves-effect', 'waves-light', 'btn', 'btn-primary' ) ),
    '#validate' => array( 'add_document_form_validate' ),
    '#submit' => array( 'add_document_form_submit' ),
  );
  $form[ 'cancel' ] = array(
    '#type' => 'submit',
    '#value' => t( 'Cancel' ),
    '#access' => TRUE,
    '#weight' => 15,
    '#attributes' => array( 'class' => array( 'waves-effect', 'waves-light', 'btn', 'btn-primary' ) ),
    '#submit' => array( 'add_document_form_cancel' ),
    '#validate' => array( 'add_document_form_validate_cancel' ),
    '#limit_validation_errors' => array(),
  );
  return $form;
}
