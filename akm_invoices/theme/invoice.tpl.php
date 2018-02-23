<?php
/**
 * @file
 * Default output for a invoice mail node.
 *
 * Avaliable variables
 * $user_name
 * $user_email
 * $start_date
 * $stop_date
 * $expiration_date
 * $subsripion_type
 * $invoice_price
 * $invoice_number
 *
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
  <title>Gold Coast Escorts</title>
  <style type="text/css">
    * {
      font-family: 'helvetica';
      margin: 0px;
      padding: 0px;
    }

    .to_username h1 {
      font-size: 24px;
      font-weight: normal;
      color: #3f343a;
      text-transform: uppercase;
    }

    .to_username p {
      font-size: 14px;
      color: #d45394;
    }

    .invoice_info h2 {
      font-size: 18px;
      font-weight: normal;
      color: #3f343a;
      text-transform: uppercase;
    }

    .invoice_info p {
      font-size: 14px;
      color: #d45394;
    }

    .invoice_left h2 {
      color: #3f343a;
      font-weight: normal;
      font-size: 18px;
      margin: 0 0 5px;
    }

    .invoice_prices h2 {
      color: #3f343a;
      font-weight: normal;
      font-size: 18px;
      padding: 0 0 5px;
    }

    .description {
      background: url(sites/all/modules/custom/akm_invoices/theme/invoice/large_box.png) no-repeat left top;
      width: 558px;
      padding: 15px;
      height: 80px;
    }

    .price_box {
      background: url(sites/all/modules/custom/akm_invoices/theme/invoice/price_box.png) no-repeat center top;
      width: 90px;
      height: 22px;
      text-align: center;
      padding: 30px;
    }

    .price_box p {
      font-size: 16px;
      color: #d45394;
      font-family: 'BrushScriptStd';
      text-align: center;
    }

    .price_box.total p {
      color: #3f343a;
    }

    .bottom_box_left {
      background: url(sites/all/modules/custom/akm_invoices/theme/invoice/box.png) no-repeat left top;
      width: 260px;
      height: 270px;
      padding: 15px;
    }

    .bottom_box_right {
      background: url(sites/all/modules/custom/akm_invoices/theme/invoice/box_right.png) no-repeat right top;
      width: 231px;
      height: 270px;
      padding: 15px 33px;
    }

    .balance {
      height: 270px;
    }

    .balance h2 {
      color: #3f343a;
      font-weight: normal;
      font-size: 18px;
      padding: 190px 0 5px;
    }

    .date {
      color: #d45394;
    }

    .bottom_box_right p {
      margin: 0 0 15px;
    }

    .bank_dates {
      color: #d45394;
    }

    .info {
      color: #ae95a2;
    }
  </style>
</head>
<body>
<table align="center" style="width: 755px">
  <tr>
    <td colspan="3" style="height: 50px;">
      &nbsp;
    </td>
  </tr>
  <tr>
    <td class="to_username" colspan="2">
      <h1>To: <?php print $user_name; ?></h1>

      <p><?php print $user_email; ?></p>
    </td>
    <td class="invoice_info">
      <h2>Invoice #</h2>

      <p><?php print $invoice_number; ?></p>

      <h2>Date:</h2>

      <p><?php print $start_date; ?></p>
    </td>
  </tr>
  <tr>
    <td colspan="3" style="height: 20px;">
      &nbsp;
    </td>
  </tr>
  <tr>
    <td colspan="2" class="invoice_left">
      <h2>Description:</h2>
    </td>
    <td class="invoice_prices" valign="bottom">
      <h2>Amount</h2>
    </td>
  </tr>
  <tr>
    <td colspan="2" class="description">
      <p>Advertise on GoldCoastEscorts.com.au (<?php print $start_date; ?> - <?php print $stop_date; ?>)</p>
    </td>
    <td>
      <table style="border-collapse: collapse;">
        <tr>
          <td class="price_box" valign="middle">
            <p><?php print $invoice_price; ?></p>
          </td>
        </tr>
        <tr>
          <td class="invoice_prices" style="padding-top: 6px;">
            <h2>Total</h2>
          </td>
        </tr>
        <tr>
          <td class="price_box total" valign="middle">
            <p><?php print $invoice_price; ?></p>
          </td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td colspan="3" style="height: 20px;">
      &nbsp;
    </td>
  </tr>
  <tr>
    <td class="bottom_box_left" valign="top">
      <p>Payment due Date</p>

      <p class="date"><?php print $expiration_date; ?></p>
    </td>
    <td valign="top">
      <div class="bottom_box_right">
        <p>Please make your payment<br/>by direct deposit or over the<br/>counter at any Suncorp Bank:</p>

        <p class="bank_dates">Suncorp Bank<br/>BSB: 484799<br/>ACC: 45-201269-3<br/>ACC Name: Brisbane Directory</p>

        <p class="info">Please use your user name as Deposit ID reference when making your deposit.</p>
      </div>
    </td>
    <td class="balance">
      <table style="border-collapse: collapse;">
        <tr>
          <td>
            <h2>Balance Due</h2>
          </td>
        </tr>
        <tr>
          <td class="price_box total" valign="middle">
            <p><?php print $invoice_price; ?></p>
          </td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td colspan="3" style="height: 20px;">
      &nbsp;
    </td>
  </tr>
</table>
</body>
</html>
