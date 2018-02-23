<?php

/*
 * Implements agent client form validations1.
 */
function agent_client_form_validate1( &$form, &$form_state ) {
  $values = $form_state[ 'values' ];
  $email = $values[ 'account' ][ 'email' ];
  $telephone = $values[ 'account' ][ 'telephone' ];
  $promo_code = $values[ 'account' ][ 'promo_code' ];

  if ( !valid_email_address( $email ) ) {
    form_set_error( 'account][email', t( 'The e-mail address %mail is not valid.' ) );
  }
  //Referral Code  - Restrict size to 10 characters.
  if ( !empty( $promo_code ) && !filter_var( $promo_code, FILTER_VALIDATE_REGEXP, array(
        "options" => array(
          "regexp" => "/^[a-zA-Z0-9]{0,10}+$/"
        )
      ) ) ) {
    form_set_error( 'account][promo_code', t( 'Referral Code - This is not a valid Referral Code.' ) );
  }
  // Telephone - Only allow numbers and phone characters such as (-).
  if ( !empty( $telephone ) && !filter_var( $telephone, FILTER_VALIDATE_REGEXP, array(
        "options" => array(
          "regexp" => "/^(\(?\+?[0-9]*\)?)?[0-9_\- \(\)]*$/"
        )
      ) ) ) {
    form_set_error( 'account][telephone', t( 'Telephone - Only numbers and phone characters such as ()+- are allowed.' ) );
  }
}

/*
 * Implements agent client form submit.
 */
function agent_client_form_submit1( $form, &$form_state ) {
  $values = $form_state[ 'values' ][ 'account' ];
  $clientID = $values[ 'clientID' ];
  $user_name = !empty( $values[ 'user_name' ] ) ? $values[ 'user_name' ] : NULL;
  $email = !empty( $values[ 'email' ] ) ? $values[ 'email' ] : NULL;
  $promo_code = !empty( $values[ 'promo_code' ] ) ? $values[ 'promo_code' ] : NULL;
  $telephone = !empty( $values[ 'telephone' ] ) ? $values[ 'telephone' ] : NULL;
  $emailnotifications = $values[ 'emailnotifications' ];

  global $user;
  $agentid = $user->uid;
  $agentUserData = user_load( $agentid );
  (intval( trim( $agentUserData->field_sugar_agent_id[ 'und' ][ 0 ][ 'value' ] ) ) > 0) ? $sugar_agent_hash_id = trim( $agentUserData->field_sugar_agent_id[ 'und' ][ 0 ][ 'value' ] ) : $sugar_agent_hash_id = 0;

  $check_client = user_load( $clientID );
  $success = true;
  if ( intval( $clientID ) > 0 && is_object( $check_client ) ) {
    try {
      /* Here We are Update profile tab first section data into application */
      $update = db_update( 'application' )
          ->condition( 'uid', $clientID, '=' )
          ->fields( array(
            'promo_code' => $promo_code,
            'mail' => $email,
            'telephone' => $telephone,
            'step1' => 1,
          ) )
          ->execute();
    }
    catch ( Exception $e ) {
      $success = false;
      drupal_set_message( t( 'Sorry, It\'s failed to update account information.' ), 'error' );
    }
    if ( $success ) {
      $query = db_select( 'application', 'o' );
      $query->condition( 'o.uid', $clientID, '=' )->fields( 'o' );
      $sugar_postresult = $query->execute()->fetchAssoc();
      $data = array(
        'promo_code' => $sugar_postresult[ 'promo_code' ],
        'email' => $sugar_postresult[ 'mail' ],
        'telephone' => $sugar_postresult[ 'telephone' ],
        'sugar_agent_hash_id' => $sugar_postresult[ 'sugar_agent_hash_id' ],
        'from_agent' => 1,
        'sugar_client_id' => $sugar_postresult[ 'sugar_client_id' ],
      );
      drupal_set_message( t( 'Account information is updated!' ) );
      $sugar_return = _drupal_to_sugar_application_api( $data, "Profile-Tab(Account)", $clientID );
      $sugar_return = ( array ) $sugar_return;
      if ( is_array( $sugar_return ) && count( $sugar_return ) > 0 ) {
        if ( $sugar_return[ 'Student' ]->user_id == $clientID ) {
          $query = db_update( 'application' )
              ->fields( array( 'sugar_client_id' => $sugar_return[ 'Student' ]->id ) )
              ->condition( 'uid', $clientID, '=' )
              ->execute();
        }
        else {
          drupal_set_message( t( 'Drupal User ID or api return user ID mismatch!' ), 'error' );
        }
      }
      drupal_goto( 'agent/edit/' . $clientID . '/client' );
    }
  }
  else {
    $firstname = explode( '@', $email );
    $existing_account_by_name = user_load_by_name( $firstname[ 0 ] );
    if ( !empty( $existing_account_by_name->uid ) ) {
      drupal_set_message( t( 'User Name already exit!' ), 'error' );
      return;
    }
    $existing_account_by_mail = user_load_by_mail( $email );
    if ( !empty( $existing_account_by_mail->uid ) ) {
      drupal_set_message( t( 'Email already exit' ), 'error' );
      return;
    }
    $newUser = array( 'name' => $firstname[ 0 ], 'pass' => 'client@1234', 'mail' => $email, 'status' => 1, 'init' => $email, );
    $newuser = user_save( null, $newUser );
    if ( is_object( $newuser ) && $emailnotifications == 1 ) {
      $client_account = user_load( $newuser->uid );
      $return_mail = _user_mail_notify( 'register_admin_created', $client_account );
    }

    /* Here We are insert profile tab first section data into application */
    try {
      $insert = db_insert( 'application' )
          ->fields( array(
            'uid' => $newuser->uid,
            'promo_code' => $promo_code,
            'mail' => $email,
            'telephone' => $telephone,
            'agentuid' => $agentid,
            'date_created' => format_date( time(), 'custom', 'Y-m-d H:i:s' ),
            'sugar_agent_hash_id' => $sugar_agent_hash_id,
            'step1' => 1,
          ) )
          ->execute();
    }
    catch ( Exception $e ) {
      $success = false;
      drupal_set_message( t( 'Sorry, It\'s failed to save account information.' ), 'error' );
    }
    if ( $success ) {
      $query = db_select( 'application', 'o' );
      $query->condition( 'o.uid', $newuser->uid, '=' )->fields( 'o' );
      $sugar_postresult = $query->execute()->fetchAssoc();
      $data = array(
        'promo_code' => $sugar_postresult[ 'promo_code' ],
        'email' => $sugar_postresult[ 'mail' ],
        'telephone' => $sugar_postresult[ 'telephone' ],
        'sugar_agent_hash_id' => $sugar_postresult[ 'sugar_agent_hash_id' ],
        'from_agent' => 1,
      );
      $sugar_return = _drupal_to_sugar_application_api( $data, "Profile-Tab(Account)", $newuser->uid );
      $sugar_return = ( array ) $sugar_return;
      drupal_set_message( t( 'Account create successfully!' ) );
      if ( is_array( $sugar_return ) && count( $sugar_return ) > 0 ) {
        if ( $sugar_return[ 'Student' ]->user_id == $newuser->uid ) {
          drupal_set_message( t( 'Account information is updated!' ) );
          $query = db_update( 'application' )
              ->fields( array( 'sugar_client_id' => $sugar_return[ 'Student' ]->id ) )
              ->condition( 'uid', $newuser->uid, '=' )
              ->execute();
        }
        else {
          drupal_set_message( t( 'Drupal User ID or api return user ID mismatch!' ), 'error' );
        }
      }
    }
    if ( $newuser->uid ) {
      drupal_goto( 'agent/edit/' . $newuser->uid . '/client' );
    }
  }
}

/*
 * Implements agent client form validation2.
 */
function agent_client_form_validate2( $form, &$form_state ) {
  $basic = $form_state[ 'values' ][ 'basic' ];
  $step1 = $basic[ 'step1' ];
  if ( $step1 == 0 ) {
    form_set_error( 'basic][step1', t( 'Please first complete account section' ) );
  }
  $passport_exp_date = str_replace( '/', '-', $basic[ 'passport_expdate' ] );
  $DOB = str_replace( '/', '-', $basic[ 'birthdate' ] );
  $date_of_birth = format_date( strtotime( "$DOB" ), 'custom', 'd-m-Y' );
  $birth_diff = "";
  $birth_diff = calculate_date_of_birth( $date_of_birth );
  if ( $birth_diff < 15 ) {
    form_set_error( 'basic][birthdate', t( 'Date of Birth - You must be 15 or older to apply.' ) );
  }
  $passport_expiry_date = format_date( strtotime( "$passport_exp_date" ), 'custom', 'd-m-Y' );
  if ( $passport_expiry_date != '' ) {
    $current_date = strtotime( format_date( time(), 'custom', 'd-m-Y' ) );
    if ( $current_date > strtotime( "$passport_expiry_date" ) ) {
      form_set_error( 'basic][passport_expdate', t( 'Passport Expiry Date - Expiry date cannot be less than today.' ) );
    }
  }
}

/*
 * Implement agent client form submit2.
 */
function agent_client_form_submit2( $form, &$form_state ) {
  $values = $form_state[ 'values' ][ 'basic' ];
  $clientID = $values[ 'clientID' ];
  $firstname = !empty( $values[ 'first_name' ] ) ? $values[ 'first_name' ] : NULL;
  $surname = !empty( $values[ 'surname' ] ) ? $values[ 'surname' ] : NULL;
  if ( !empty( $values[ 'country' ] ) ) {
    $country = fetchCodeByValueForCountry( $values[ 'country' ] );
  }
  else {
    $country = NULL;
  }
  if ( !empty( $values[ 'nationality' ] ) ) {
    $nationality = fetchCodeByValueForNationalities( $values[ 'nationality' ] );
  }
  else {
    $nationality = NULL;
  }

  $passport_number = !empty( $values[ 'passport_number' ] ) ? $values[ 'passport_number' ] : NULL;
  ($values[ 'gender' ] == '') ? $gender = NULL : $gender = $values[ 'gender' ];

  $address = !empty( $values[ 'address' ] ) ? $values[ 'address' ] : NULL;
  $city = !empty( $values[ 'city' ] ) ? $values[ 'city' ] : NULL;
  $district = !empty( $values[ 'district' ] ) ? $values[ 'district' ] : NULL;
  $zip = !empty( $values[ 'zipcode' ] ) ? $values[ 'zipcode' ] : NULL;
  $DOB = str_replace( '/', '-', $values[ 'birthdate' ] );
  $birth_date = strtotime( $DOB );
  if ( $values[ 'passport_expdate' ] != '' ) {
    $passport_exp_date = str_replace( '/', '-', $values[ 'passport_expdate' ] );
    $passport_expiry_date = strtotime( $passport_exp_date );
  }
  else {
    $passport_expiry_date = '0';
  }
  global $user;
  $query = db_select( 'application', 'o' );
  $query->condition( 'o.uid', $clientID, '=' );
  $query->fields( 'o', array( 'uid' ) );
  $result = $query->execute();
  $has_info = false;
  foreach ( $result as $record ) {
    $has_info = true;
  }
  $success = true;
  if ( $has_info ) {
    try {
      /* Here We are Update profile tab II section data into application */
      $update = db_update( 'application' )
          ->condition( 'uid', $clientID, '=' )
          ->fields( array(
            'firstname' => $firstname,
            'surname' => $surname,
            'country' => $country,
            'nationality' => $nationality,
            'passport_number' => $passport_number,
            'passport_expiry_date' => $passport_expiry_date,
            'birthday' => $birth_date,
            'gender' => $gender,
            'address' => $address,
            'city' => $city,
            'district' => $district,
            'zip' => $zip,
            'step2' => 1,
          ) )
          ->execute();
    }
    catch ( Exception $e ) {
      $success = false;
      drupal_set_message( t( 'Sorry, It\'s failed to update account information.' ), 'error' );
    }
    if ( $success ) {
      $query = db_select( 'application', 'o' );
      $query->condition( 'o.uid', $clientID, '=' )->fields( 'o' );
      $sugar_postresult = $query->execute()->fetchAssoc();

      if ( !empty( $sugar_postresult[ 'birthday' ] ) ) {
        $birthday = array();
        $birth_date = date( 'd/m/Y', $sugar_postresult[ 'birthday' ] );
        $birth_date_array = explode( '/', $birth_date );
        $birthday[ 'day' ] = $birth_date_array[ '0' ];
        $birthday[ 'month' ] = $birth_date_array[ '1' ];
        $birthday[ 'year' ] = $birth_date_array[ '2' ];
      }

      if ( !empty( $sugar_postresult[ 'passport_expiry_date' ] ) ) {

        $passport_expiry_date = array();
        $passport_expirydate = date( 'd/m/Y', $sugar_postresult[ 'passport_expiry_date' ] );
        $passport_expirydate_array = explode( '/', $passport_expirydate );
        $passport_expiry_date[ 'day' ] = $passport_expirydate_array[ '0' ];
        $passport_expiry_date[ 'month' ] = $passport_expirydate_array[ '1' ];
        $passport_expiry_date[ 'year' ] = $passport_expirydate_array[ '2' ];
      }
      $data = array(
        'first_name' => $sugar_postresult[ 'firstname' ],
        'surname' => $sugar_postresult[ 'surname' ],
        'country' => $sugar_postresult[ 'country' ],
        'nationality' => $sugar_postresult[ 'nationality' ],
        'passport_number' => $sugar_postresult[ 'passport_number' ],
        'expiry' => $passport_expiry_date,
        'birth' => $birthday,
        'gender' => $sugar_postresult[ 'gender' ],
        'address' => $sugar_postresult[ 'address' ],
        'city' => $sugar_postresult[ 'city' ],
        'district' => $sugar_postresult[ 'district' ],
        'zipcode' => $sugar_postresult[ 'zip' ],
        'sugar_client_id' => $sugar_postresult[ 'sugar_client_id' ],
      );
      $sugar_return = _drupal_to_sugar_application_api( $data, "Profile-Tab(Basic)", $clientID );
      $sugar_return = ( array ) $sugar_return;
      if ( is_array( $sugar_return ) && count( $sugar_return ) > 0 ) {
        if ( $sugar_return[ 'Student' ]->user_id == $clientID ) {
          drupal_set_message( t( 'Basic information is updated!' ) );
        }
        else {
          drupal_set_message( t( 'Drupal User ID or api return user ID mismatch!' ), 'error' );
        }
      }
      else {
        drupal_set_message( t( 'Something worng crm api is not responding' ), 'error' );
      }
    }
    drupal_goto( 'agent/edit/' . $clientID . '/client' );
  }
  else {
    /* Here We are insert profile tab II section data into application */
    drupal_set_message( t( 'Please first complete account section.' ), 'error' );
  }
}

/*
 * Implements agent client form validations2.
 */
function agent_client_form_validate3( $form, &$form_state ) {
  $values = $form_state[ 'values' ][ 'acadimic' ];
  $basic = $form_state[ 'values' ][ 'basic' ];
  $step1 = $basic[ 'step1' ];
  if ( $step1 == 0 ) {
    form_set_error( 'acadimic][step1', t( 'Please first complete account section' ) );
  }
  $step2 = $values[ 'step2' ];
  if ( $step2 == 0 ) {
    form_set_error( 'acadimic][step2', t( 'Please first complete basic section' ) );
  }
}

/*
 * Implement agent client form submit3.
 */
function agent_client_form_submit3( $form, &$form_state ) {
  $values = $form_state[ 'values' ][ 'acadimic' ];
  $clientID = $values[ 'clientID' ];
  $current_course_level = !empty( $values[ 'current_course_level' ] ) ? $values[ 'current_course_level' ] : NULL;
  $current_institution = !empty( $values[ 'current_institution' ] ) ? $values[ 'current_institution' ] : NULL;
  $current_course = !empty( $values[ 'current_course' ] ) ? $values[ 'current_course' ] : NULL;
  ($values[ 'gpa_score' ] == '') ? $gpa_score = NULL : $gpa_score = $values[ 'gpa_score' ];
  ($values[ 'gpa_type' ] == '') ? $gpa_type == NULL : $gpa_type = $values[ 'gpa_type' ];

  $query = db_select( 'application', 'o' );
  $query->condition( 'o.uid', $clientID, '=' );
  $query->fields( 'o', array( 'uid' ) );
  $result = $query->execute();
  $has_info = false;
  foreach ( $result as $record ) {
    $has_info = true;
  }
  $success = true;
  if ( $has_info ) {
    try {
      /* Here We are Update profile tab III section data into application */
      $update = db_update( 'application' )
          ->condition( 'uid', $clientID, '=' )
          ->fields( array(
            'current_institution' => $current_institution,
            'current_course' => $current_course,
            'current_course_level' => $current_course_level,
            'gpa_type' => $gpa_type,
            'gpa_score' => $gpa_score,
            'step3' => 1,
          ) )
          ->execute();
    }
    catch ( Exception $e ) {
      $success = false;
      drupal_set_message( t( 'Sorry, It\'s failed to update academic information.' ), 'error' );
    }
    if ( $success ) {
      $query = db_select( 'application', 'o' );
      $query->condition( 'o.uid', $clientID, '=' )->fields( 'o' );
      $sugar_postresult = $query->execute()->fetchAssoc();
      $data = array(
        'current_institution' => $sugar_postresult[ 'current_institution' ],
        'current_course' => $sugar_postresult[ 'current_course' ],
        'current_course_level' => $sugar_postresult[ 'current_course_level' ],
        'gpa_type' => $sugar_postresult[ 'gpa_type' ],
        'gpa_score' => $sugar_postresult[ 'gpa_score' ],
        'sugar_client_id' => $sugar_postresult[ 'sugar_client_id' ],
      );
      $sugar_return = _drupal_to_sugar_application_api( $data, "Profile-Tab(Acadimic)", $clientID );
      $sugar_return = ( array ) $sugar_return;
      if ( is_array( $sugar_return ) && count( $sugar_return ) > 0 ) {
        if ( $sugar_return[ 'Student' ]->user_id == $clientID ) {
          drupal_set_message( t( 'Academic information is updated!' ) );
        }
        else {
          drupal_set_message( t( 'Drupal User ID or api return user ID mismatch!' ), 'error' );
        }
      }
      else {
        drupal_set_message( t( 'Something worng crm api is not responding' ), 'error' );
      }
      drupal_goto( 'agent/edit/' . $clientID . '/client' );
    }
  }
  else {
    drupal_set_message( t( 'Please first complete account section.' ), 'error' );
  }
}

/*
 * Implements agent client form validations4.
 */
function agent_client_form_validate4( $form, &$form_state ) {
  $values = $form_state[ 'values' ][ 'english' ];
}

/*
 * Implement agent client form submit4.
 */
function agent_client_form_submit4( $form, &$form_state ) {
  $values = $form_state[ 'values' ][ 'english' ];
  $clientID = $values[ 'clientID' ];
  $test_type = trim( $values[ 'test_type' ] );
  if ( $test_type == '' ) {
    $test_type = -1;
  }
  $test_score = trim( $values[ 'test_score' ] );
  if ( $test_score == '' ) {
    $test_score = -1;
  }

  $speaking_test_score = trim( $values[ 'speaking_test_score' ] );
  if ( $speaking_test_score == '' ) {
    $speaking_test_score = -1;
  }

  $writing_test_score = trim( $values[ 'writing_test_score' ] );
  if ( $writing_test_score == '' ) {
    $writing_test_score = -1;
  }
  $listening_test_score = trim( $values[ 'listening_test_score' ] );
  if ( $listening_test_score == '' ) {
    $listening_test_score = -1;
  }
  $reading_test_score = trim( $values[ 'reading_test_score' ] );
  if ( $reading_test_score == '' ) {
    $reading_test_score = -1;
  }
  if ( $test_type == 0 ) {
    $test_score = -1;
    $speaking_test_score = -1;
    $writing_test_score = -1;
    $listening_test_score = -1;
    $reading_test_score = -1;
    $step4 = 1;
  }
  else {
    $step4 = 1;
  }
  if ( $test_type == '' ) {
    $step4 = 1;
  }
  $query = db_select( 'application', 'o' );
  $query->condition( 'o.uid', $clientID, '=' );
  $query->fields( 'o', array( 'uid' ) );
  $result = $query->execute();
  $has_info = false;
  foreach ( $result as $record ) {
    $has_info = true;
  }
  $success = true;
  if ( $test_type != NULL ) {
    if ( $has_info ) {
      try {
        /* Here We are Update profile tab III section data into application */
        $update = db_update( 'application' )
            ->condition( 'uid', $clientID, '=' )
            ->fields( array(
              'test_type' => $test_type,
              'test_score' => $test_score,
              'speaking_test_score' => $speaking_test_score,
              'writing_test_score' => $writing_test_score,
              'listening_test_score' => $listening_test_score,
              'reading_test_score' => $reading_test_score,
              'step4' => $step4,
            ) )
            ->execute();
      }
      catch ( Exception $e ) {
        $success = false;
        drupal_set_message( t( 'Sorry, It\'s failed to update English language information.' ), 'error' );
      }
      if ( $success ) {
        $query = db_select( 'application', 'o' );
        $query->condition( 'o.uid', $clientID, '=' )->fields( 'o' );
        $sugar_postresult = $query->execute()->fetchAssoc();
        $data = array(
          'test_type' => $sugar_postresult[ 'test_type' ],
          'test_score' => $sugar_postresult[ 'test_score' ],
          'speaking_test_score' => $sugar_postresult[ 'speaking_test_score' ],
          'writing_test_score' => $sugar_postresult[ 'writing_test_score' ],
          'listening_test_score' => $sugar_postresult[ 'listening_test_score' ],
          'reading_test_score' => $sugar_postresult[ 'reading_test_score' ],
          'sugar_client_id' => $sugar_postresult[ 'sugar_client_id' ],
        );
        $sugar_return = _drupal_to_sugar_application_api( $data, "Profile-Tab(English)", $clientID );
        $sugar_return = ( array ) $sugar_return;
        if ( is_array( $sugar_return ) && count( $sugar_return ) > 0 ) {
          if ( $sugar_return[ 'Student' ]->user_id == $clientID ) {
            drupal_set_message( t( 'English language information is updated!' ) );
          }
          else {
            drupal_set_message( t( 'Drupal User ID or api return user ID mismatch!' ), 'error' );
          }
        }
        else {
          drupal_set_message( t( 'Something worng crm api is not responding' ), 'error' );
        }
        drupal_goto( 'agent/edit/' . $clientID . '/client' );
      }
    }
    else {
      drupal_set_message( t( 'Please first complete account section.' ), 'error' );
    }
  }
}

/*
 * Implement agent client form validation5.
 */
function agent_client_form_validate5( $form, &$form_state ) {
  $values = $form_state[ 'values' ][ 'finance' ];
  $fees_budget = $values[ 'fees_budget' ];
  $scholarship = $values[ 'scholarship' ];
  $preferred_currency = $values[ 'preferred_currency' ];
  if ( trim( $fees_budget ) != '' && $preferred_currency == '' ) {
    form_set_error( 'finance][preferred_currency', t( 'Preferred Currency - Please select Preferred Currency.' ) );
  }
  if ( trim( $fees_budget ) == '' && $preferred_currency != '' ) {
    form_set_error( 'finance][fees_budget', t( 'Fees Budget - Please Enter Fees Budget.' ) );
  }
  if ( !empty( $fees_budget ) && !filter_var( $fees_budget, FILTER_VALIDATE_REGEXP, array(
        "options" => array(
          "regexp" => "/^[0-9.]+$/"
        )
      ) ) ) {
    form_set_error( 'finance][fees_budget', t( 'Fees Budget - This is not a valid Number.' ) );
  }
  if ( $fees_budget > '50000' || $fees_budget < '0' ) {
    form_set_error( 'finance][fees_budget', t( 'Fees Budget - Fees is out of Budget, please recheck again.' ) );
  }
  if ( !empty( $scholarship ) && !filter_var( $scholarship, FILTER_VALIDATE_REGEXP, array(
        "options" => array(
          "regexp" => "/^[0-9.]+$/"
        )
      ) ) ) {
    form_set_error( 'finance][scholarship', t( 'Scholarship - This is not a valid Number.' ) );
  }
}

/*
 * Implement agent client form submit5.
 */
function agent_client_form_submit5( $form, &$form_state ) {
  $values = $form_state[ 'values' ][ 'finance' ];
  $clientID = $values[ 'clientID' ];
  (trim( $values[ 'fees_budget' ] ) == '') ? $fees_budget = 0 : $fees_budget = $values[ 'fees_budget' ];
  (trim( $values[ 'scholarship' ] ) == '') ? $scholarship = 0 : $scholarship = $values[ 'scholarship' ];
  if ( $fees_budget == 0 && $scholarship == 0 && $preferred_currency == '' ) {
    $step5 = 0;
  }
  else {
    $step5 = 1;
  }
  $preferred_currency = $values[ 'preferred_currency' ];
  $query = db_select( 'application', 'o' );
  $query->condition( 'o.uid', $clientID, '=' );
  $query->fields( 'o', array( 'uid' ) );
  $result = $query->execute();
  $has_info = false;
  foreach ( $result as $record ) {
    $has_info = true;
  }
  $success = true;
  if ( $has_info ) {
    try {
      /* Here We are Update profile tab Finance section data into application */
      $update = db_update( 'application' )
          ->condition( 'uid', $clientID, '=' )
          ->fields( array(
            'fees_budget' => $fees_budget,
            'scholarship' => $scholarship,
            'preferred_currency' => $preferred_currency,
            'step5' => $step5,
          ) )
          ->execute();
    }
    catch ( Exception $e ) {
      $success = false;
      drupal_set_message( t( 'Sorry, It\'s failed to update Finance information.' ), 'error' );
    }
    if ( $success ) {
      drupal_set_message( t( 'Finance information is updated!' ) );   
      drupal_goto( 'agent/clients' );
    }
  }
  else {
    drupal_set_message( t( 'Please first complete account section.' ), 'error' );
  }
}

/*
 * Implement agent client form cancel.
 */
function agent_client_form_cancel( $form, &$form_state ) {
  $url = $_GET[ 'destination' ] ? $_GET[ 'destination' ] : 'agent/clients';
  drupal_goto( $url );
}

/*
 * Implement add_application_form_validate().
 */
function add_application_form_validate( $form, &$form_state ) {
  $values = $form_state[ 'values' ];
  $appindex_hash = explode( '-', strip_tags( $values[ 'op' ] ) );
  $appindex = base64_decode( $appindex_hash[ 1 ] );
  $status_nid_uni = $values[ 'courses' ][ $appindex ][ 'institution_id' ];
  $status_cnid = $values[ 'courses' ][ $appindex ][ 'course_id' ];
  $status_intake = $values[ 'courses' ][ $appindex ][ 'intake' ];
  $status_entry_year = $values[ 'courses' ][ $appindex ][ 'entry_year' ];
  $status_pre_sessional_english = $values[ 'courses' ][ $appindex ][ 'pre_sessional_english' ];
  $client = $values[ 'courses' ][ $appindex ][ 'client' ];
  $flag = true;
  if ( trim( $status_pre_sessional_english ) == '' || trim( $status_entry_year ) == '' || trim( $status_intake ) == '' || trim( $status_cnid ) == '' || trim( $status_nid_uni ) == '' || $client == 0 ) {
    $flag = false;
    form_set_error( 'institution', t( 'Please Select Institutions & Courses,Intake,Entry Year,Client' ) );
  }
}

/*
 * Implement add_application_form_submit().
 */
function add_application_form_submit( $form, &$form_state ) {
  $values = $form_state[ 'values' ];

  $appindex_hash = explode( '-', strip_tags( $values[ 'op' ] ) );
  $appindex = base64_decode( $appindex_hash[ 1 ] );

  $clientID = $values[ 'courses' ][ $appindex ][ 'client' ];

  $query = db_select( 'application_step2', 'os' );
  $query->fields( 'os', array( 'unid', 'status', 'id' ) );
  $query->condition( 'os.uid', $clientID, '=' );
  $result = $query->execute();

  $nid_uni = $values[ 'courses' ][ $appindex ][ 'institution_id' ];
  $cnid = $values[ 'courses' ][ $appindex ][ 'course_id' ];
  $intake = $values[ 'courses' ][ $appindex ][ 'intake' ];
  $status = 'Incomplete Application';
  $pre_sessional_english = $values[ 'courses' ][ $appindex ][ 'pre_sessional_english' ];
  $entry_year = $values[ 'courses' ][ $appindex ][ 'entry_year' ];
  $appid = $values[ 'courses' ][ $appindex ][ 'application_id' ];


  $insert_row = array();
  $success = true;
  if ( intval( $nid_uni ) > 0 && intval( $cnid ) > 0 ) {
    $insert_row = array(
      'uid' => $clientID,
      'unid' => !empty( $nid_uni ) ? $nid_uni : NULL,
      'cnid' => !empty( $cnid ) ? $cnid : NULL,
      'intake' => $intake,
      'entry_year' => $entry_year,
      'pre_sessional_english' => $pre_sessional_english,
      'status' => $status,
    );
    if ( intval( $appid ) > 0 ) {
      try {
        $last_insert_id = $appid;
        $update = db_update( 'application_step2' )
            ->condition( 'id', $appid, '=' )
            ->fields( $insert_row )
            ->execute();
      }
      catch ( Exception $e ) {
        $success = false;
        drupal_set_message( t( 'Sorry, It\'s failed to update account information.' ), 'error' );
      }
    }
    else {
      try {
        $insert_row[ 'date_created' ] = format_date( time(), 'custom', 'Y-m-d H:i:s' );
        $last_insert_id = db_insert( 'application_step2' )
            ->fields( $insert_row )
            ->execute();
      }
      catch ( Exception $e ) {
        $success = false;
        drupal_set_message( t( 'Sorry, It\'s failed to save institutions & courses.' ), 'error' );
      }
    }
    if ( $success ) {
      drupal_set_message( 'Institutions & courses are saved.' );
      $query = db_select( 'application_step2', 'o_st' );
      $query->condition( 'o_st.uid', $clientID, '=' );
      $query->condition( 'o_st.id', $last_insert_id, '=' );
      $query->fields( 'o_st', array( 'id', 'unid', 'cnid', 'intake', 'entry_year', 'pre_sessional_english', 'status', 'sugar_application_identifier' ) );
      $ukuni_app_data = $query->execute()->fetchAll();
      foreach ( $ukuni_app_data as $key => $val ) {
        $ukuni_app_data[ $key ] = ( array ) $ukuni_app_data[ $key ];
      }
      $sugar_postdata[ 'courses' ] = $ukuni_app_data;
      /* Here We are post data into sugar CRM */
      $sugar_return = _drupal_to_sugar_application_api( $sugar_postdata, "STEP-2(Add Application)", $clientID );
      $sugar_return = ( array ) $sugar_return;
      if ( intval( $sugar_return[ 'user_id' ] ) == $clientID ) {
        if ( is_array( $sugar_return[ 'IA' ] ) && count( $sugar_return[ 'IA' ] ) > 0 ) {
          foreach ( $sugar_return[ 'IA' ] as $sugar_return_key => $sugar_return_val ) {
            $id = $sugar_return_val->ukuni_application_id;
            $sugar_app_id = $sugar_return_val->id;
            $query = db_select( 'application_step2', 'o_st' );
            $query->condition( 'o_st.id', $id, '=' );
            $query->fields( 'o_st', array( 'sugar_application_identifier' ) );
            $ukuniappdata = $query->execute()->fetchAssoc();
            if ( $ukuniappdata[ 'sugar_application_identifier' ] == '' ) {
              db_update( 'application_step2' )->fields( array( 'sugar_application_identifier' => $sugar_app_id ) )->condition( 'id', $id )->execute();
            }
            $i++;
          }
        }
      }
      $redirect = 'agent/client/add/' . $clientID . '/application';
      drupal_goto( $redirect );
    }
  }
  else {
    drupal_set_message( t( 'Some Required Field Value Missing' ), 'error' );
    $redirect = 'agent/client/add/' . $clientID . '/application';
    drupal_goto( $redirect );
  }
}

/*
 * Implement add_application_form_cancel().
 */
function add_application_form_cancel( $form, &$form_state ) {
  $url = $_GET[ 'destination' ] ? $_GET[ 'destination' ] : 'agent/applications';
  drupal_goto( $url );
}

/*
 * Implement add_application_form_save_draft_validate().
 */
function add_application_form_save_draft_validate( $form, &$form_state ) {
  $values = $form_state[ 'values' ];
  $appindex_hash = explode( '-', strip_tags( $values[ 'op' ] ) );
  $appindex = base64_decode( $appindex_hash[ 1 ] );
  $client = $values[ 'courses' ][ $appindex ][ 'client' ];
  if ( trim( $client ) == 0 ) {
    form_set_error( 'institution', t( 'Please Select Client' ) );
  }
}

/*
 * Implement add_application_form_save_draft_submit().
 */
function add_application_form_save_draft_submit( $form, &$form_state ) {
  $values = $form_state[ 'values' ];
  $appindex_hash = explode( '-', strip_tags( $values[ 'op' ] ) );
  $appindex = base64_decode( $appindex_hash[ 1 ] );
  if ( intval( $appindex ) >= 0 ) {
    (!empty( $values[ 'courses' ][ $appindex ][ 'institution_id' ] )) ? $nid_uni = $values[ 'courses' ][ $appindex ][ 'institution_id' ] : $nid_uni = NULL;
    (!empty( $values[ 'courses' ][ $appindex ][ 'course_id' ] )) ? $cnid = $values[ 'courses' ][ $appindex ][ 'course_id' ] : $cnid = NULL;
    ($values[ 'courses' ][ $appindex ][ 'intake' ] != '') ? $intake = $values[ 'courses' ][ $appindex ][ 'intake' ] : $intake = NULL;
    (trim( $values[ 'courses' ][ $appindex ][ 'pre_sessional_english' ] ) != '') ? $pre_sessional_english = $values[ 'courses' ][ $appindex ][ 'pre_sessional_english' ] : $pre_sessional_english = NULL;
    ($values[ 'courses' ][ $appindex ][ 'entry_year' ] != '') ? $entry_year = $values[ 'courses' ][ $appindex ][ 'entry_year' ] : $entry_year = NULL;
    $appid = $values[ 'courses' ][ $appindex ][ 'application_id' ];
    $clientID = $values[ 'courses' ][ $appindex ][ 'client' ];
    $status = 'Not submitted';
  }
  $insert_row = array();
  $success = true;
  if ( intval( $nid_uni ) > 0 || intval( $cnid ) > 0 || trim( $intake ) != '' || trim( $pre_sessional_english ) != '' || trim( $entry_year ) != '' ) {
    $insert_row = array(
      'uid' => $clientID,
      'unid' => $nid_uni,
      'cnid' => $cnid,
      'intake' => $intake,
      'entry_year' => $entry_year,
      'pre_sessional_english' => $pre_sessional_english,
      'status' => $status,
    );
    if ( intval( $appid ) > 0 ) {
      try {
        /* Here We are Update profile tab first section data into application */
        $update = db_update( 'application_step2' )
            ->condition( 'id', $appid, '=' )
            ->fields( $insert_row )
            ->execute();
      }
      catch ( Exception $e ) {
        $success = false;
        drupal_set_message( t( 'Sorry, It\'s failed to update account information.' ), 'error' );
      }
    }
    else {
      $insert_row[ 'date_created' ] = format_date( time(), 'custom', 'Y-m-d H:i:s' );
      try {
        $last_insert_id = db_insert( 'application_step2' )
            ->fields( $insert_row )
            ->execute();
      }
      catch ( Exception $e ) {
        $success = false;
        drupal_set_message( t( 'Sorry, It\'s failed to save institutions & courses.' ), 'error' );
      }
    }
    if ( $success ) {
      drupal_set_message( 'Institutions & courses are Saved As Draft.' );
      $redirect = 'agent/client/add/' . $clientID . '/application';
      drupal_goto( $redirect );
    }
  }
  else {
    drupal_set_message( t( 'Some required field value are missing' ), 'error' );
    $redirect = 'agent/client/add/' . $clientID . '/application';
    drupal_goto( $redirect );
  }
}

/*
 * Agent Document Section Relate Function Start
 */
function add_document_form_validate_cancel( $form, &$form_state ) {
  
}

/*
 * Create function to cancel document form
 */
function add_document_form_cancel() {
  $url = $_GET[ 'destination' ] ? $_GET[ 'destination' ] : 'agent/clients';
  drupal_goto( $url );
}

/*
 * Function for document form validations.
 */
function add_document_form_validate( $form, &$form_state ) {
  $clientid = $form_state[ 'values' ][ 'clientid' ];
  if ( trim( $clientid ) == 0 ) {
    form_set_error( 'clientid', t( 'Please Select Client' ) );
    $redirect = 'agent/client/add/document';
    drupal_goto( $redirect );
  }
  $type = $form_state[ 'values' ][ 'document_type' ];
  if ( trim( $type ) == '' ) {
    form_set_error( 'document_type', t( 'Please Select Document Type' ) );
    ($clientid) ? $redirect = 'agent/client/add/' . $clientid . '/document' : $redirect = 'agent/client/add/document';
    drupal_goto( $redirect );
  }
}

/*
 * Function for add_document_form_submit().
 */
function add_document_form_submit( $form, &$form_state ) {
  $values = $form_state[ 'values' ][ 'field_student_documents' ][ 'und' ];
  $type = $form_state[ 'values' ][ 'document_type' ];
  $clientid = $form_state[ 'values' ][ 'clientid' ];
  agent_handle_newdocuments( $type, $values, $clientid );
  _drupal_to_sugar_application_api( $form_state[ 'values' ], 'STEP-3(Upload Document)', $clientid );
  drupal_set_message( 'Documents are saved!' );
  $options = array( 'absolute' => TRUE, 'fragment' => 'tab/2' );
  $redirect = 'agent/client/add/' . $clientid . '/document';
  drupal_goto( $redirect );
}

/*
 * Implements agent_handle_newdocuments().
 */
function agent_handle_newdocuments( $type, $values, $clientid ) {
  if ( trim( $type ) != '' ) {
    $document_array = array( 'cas_document' => 'CAS Document', 'other_doc' => 'Other', 'tb_test' => 'TB Test', 'CV' => 'CV', 'nda' => 'NDA', 'ccl' => 'CCL',
      'status_cert' => 'Status Certificate', 'language_cert' => 'Language Certificate', 'approved_ps' => 'Approved PS', 'personal_statement' => 'Unapproved PS',
    );
    if ( $document_array[ $type ] ) {
      $type = $document_array[ $type ];
    }
    else {
      $type = ucwords( str_replace( "_", " ", $type ) );
    }

    if ( is_array( $values ) && count( $values ) > 0 ) {
      foreach ( $values as $key => $val ) {
        $fid = $val[ 'fid' ];
        if ( ($fid) > 0 ) {
          $doc_detail_id = db_insert( 'application_doc_details' )
                  ->fields( array(
                    'fid' => $fid,
                    'user_id' => $clientid,
                    'sugar_doc_hash_id' => '',
                    'delete_access' => 'YES',
                    'DocumentType' => "ALLAPPLICATION",
                    'DocumentTypeName' => $type,
                  ) )->execute();
          if ( intval( $doc_detail_id ) > 0 ) {
            agent_save_file_newusage( $fid, $type, $clientid );
          }
        }
      }
    }
  }
}

/*
 * Implements agent_save_file_newusage().
 */
function agent_save_file_newusage( $fid, $type, $clientid ) {
  $file = file_load( $fid );
  $file->status = FILE_STATUS_PERMANENT;
  file_usage_add( $file, 'application', $type, $clientid );
  file_save( $file );
}

/*
 * Implements agent_document_file_delete_submit_validate().
 */
function agent_document_file_delete_submit_validate( $form, &$form_state ) {
  
}

/*
 * Implements agent_document_file_delete_submit().
 */
function agent_document_file_delete_submit( $form, &$form_state ) {
  $form_state[ 'rebuild' ] = TRUE;
}

/*
 * Implements agent_document_file_delete_ajax().
 */
function agent_document_file_delete_ajax( $form, &$form_state ) {
  $fid_arry = $form_state[ 'triggering_element' ][ '#ajax' ][ 'parameters' ];
  $fid = $fid_arry[ 'fid' ];
  $clientid = $fid_arry[ 'clientid' ];
  if ( intval( $fid ) > 0 ) {
    $query = db_select( 'application_doc_details', 'appdoc_detail' );
    $query->condition( 'appdoc_detail.user_id', $clientid, '=' );
    $query->condition( 'appdoc_detail.fid', $fid, '=' );
    $query->fields( 'appdoc_detail', array( 'id', 'fid', 'sugar_doc_hash_id', 'delete_access', 'DocumentType' ) );
    $app_doc_data = $query->execute()->fetchAssoc();
    if ( is_array( $app_doc_data ) && count( $app_doc_data ) > 0 ) {

      $data[ 'fid' ] = $app_doc_data[ 'fid' ];
      $data[ 'sugar_doc_hash_id' ] = $app_doc_data[ 'sugar_doc_hash_id' ];
      $file = file_load( $app_doc_data[ 'fid' ] );
      if ( !empty( $file ) ) {
        $status = file_delete( $file, 'TRUE' );
        if ( $status == 'TRUE' ) {
          db_delete( 'application_doc_details' )->condition( 'id', $app_doc_data[ 'id' ] )->execute();
          $output .= '<script> Materialize.toast("<ul><li>Document Deleted Successfully</li></ul>", 5000, "toast alert-success"); </script>';
          $sugar_return = _drupal_to_sugar_application_api( $data, "STEP-3(Delete Document)", $clientid );
        }
      }
      else {
        $output .= '<script> Materialize.toast("<ul><li>Document does not Delete.</li></ul>", 5000, "toast alert-error"); </script>';
      }
    }
  }
  return array( '#markup' => $output );
}

/* End Agent Portal Document Sections Related Functions */

/*
 * Implements agent_ukuni_status_change().
 */
function agent_ukuni_status_change() {
  $status_value = trim( $_POST[ 'status_value' ] );
  $ukuni_application_id = $_POST[ 'ukuni_application_id' ];
  $clientid = $_POST[ 'clientid' ];
  $query = db_select( 'application_step2', 'os' );
  $query->fields( 'os', array( 'unid', 'status', 'id' ) );
  $query->condition( 'os.uid', $clientid, '=' );
  $query->condition( 'os.id', $ukuni_application_id, '=' );
  $result = $query->execute();

  foreach ( $result as $record ) {
    $has_info = true;
  }
  $success = true;

  if ( $has_info ) {
    try {
      /* Here We are Update status of that application */
      $update = db_update( 'application_step2' )
          ->condition( 'uid', $clientid, '=' )
          ->condition( 'id', $ukuni_application_id, '=' )
          ->fields( array(
            'status' => $status_value,
            'date_modified' => format_date( time(), 'custom', 'Y-m-d H:i:s' ),
          ) )
          ->execute();
    }
    catch ( Exception $e ) {
      $success = false;
      $msg[ 0 ] = 0; //'Fail to update Status of application'
    }
    if ( $success ) {
      $query = db_select( 'application_step2', 'o_st' );
      $query->condition( 'o_st.uid', $clientid, '=' );
      $query->condition( 'o_st.id', $ukuni_application_id, '=' );
      $query->fields( 'o_st', array( 'id', 'status', 'sugar_application_identifier' ) );
      $appdata_status_update = $query->execute()->fetchAssoc();
      if ( is_array( $appdata_status_update ) && count( $appdata_status_update ) > 0 ) {
        $app_status[ 'status' ][ 0 ][ 'status' ] = $appdata_status_update[ 'status' ];
        $app_status[ 'status' ][ 0 ][ 'sugar_application_identifier' ] = $appdata_status_update[ 'sugar_application_identifier' ];
        $app_status[ 'status' ][ 0 ][ 'ukuni_application_id' ] = $appdata_status_update[ 'id' ];
      }
      if ( is_array( $app_status[ 'status' ] ) && count( $app_status[ 'status' ] ) > 0 ) {
        //drupal_set_message('Application status update successfully.');
        $retun_data = _drupal_to_sugar_application_api( $app_status, "STEP-2(Status-Update)", $clientid );
      }

      $drop_status = status_dropdown( $status_value );
      $liHtml = '';
      foreach ( $drop_status as $key => $val ) {
        if ( $status_value == $key ) {
          $test[] = $key;
          $liHtml.='<li id="' . $key . '"><a href="javascript:void(0);" style="color:#cb0000!important;" >' . $val . '</a></li>';
        }
        else {
          $liHtml.='<li id="' . $key . '"><a href="javascript:void(0);" >' . $val . '</a></li>';
        }
      }

      $msg[ 0 ] = 1; //'Status updated successfully'
      $msg[ 1 ] = showAppStatus( $status_value );
      $msg[ 2 ] = $liHtml;
    }
  }
  else {
    $msg[ 0 ] = 2; //'Thier is no any record in our table'
    $msg[ 1 ] = '';
  }
  echo json_encode( $msg );
}
