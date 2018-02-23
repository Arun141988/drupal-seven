jQuery(document).ready(function ($) {
    jq111(".main_add").hide()
    jq111("#waves-effect-new").click(function () {
        jq111('#addapp-btn-msg').hide();
        jq111(".main_add").show()
        jq111(".main_add").trigger("click");
    });

    var current_page_path = window.location.pathname
    var lan = Drupal.settings.current_lang;
    if (lan == 'en') {
        lan = '';
    } else {
        lan = '/' + lan;
    }
    $('.exp').removeClass('active');
    $('#tabul').removeClass('tab3');
    $('#tabul').removeClass('tab4');
    $('#tabul').removeClass('tab5');
    $('#tabul').removeClass('tab6');

    var app = current_page_path.match(/application/g);
    var doc = current_page_path.match(/document/g);
    var editApp = current_page_path.match(/edit/g);
    var editClient = current_page_path.match(/client/g);

    if (current_page_path == lan + "/agent/clients") {
        $('#0').addClass('active');
    } else if (current_page_path == lan + "/agent/client/add/application" || (app == "application" && editClient == "client")) {
        $('#1').addClass('active');
    } else if (current_page_path == lan + "/agent/add/client" || editApp == 'edit') {
        $('.exp').removeClass('active');
        $('#3').addClass('active');
    } else if (current_page_path == lan + "/agent/client/add/document" || doc == "document") {
        $('#2').addClass('active');
    }
});

jq111(document).ready(function (jq111) {
    jq111('#tab .sp-lightbox').on('click', function (event) {
        event.preventDefault();
        if (jq111('#tab').hasClass('sp-swiping') === false) {
            jq111.fancybox.open(this);
        }
    });
    jq111(".righttopdrop").click(function (e) {
        jq111(".dropdown-content").css("display", "none");
        e.preventDefault();
        return false;
    });
});
/*Agent Portal Application Status Update*/
(function ($) {
    jQuery(document).ready(function ($) {
        $(document).on('click', '.dropdown3 li', function (e) {
            var status_value = this.id;
            var parentvalue = $(this).parent().attr('id');
            var th = $(this);
            th.closest('.test_class').removeClass("active");
            $(".collapsible-body").hide();
            var ukuni_application_id = $(this).closest('.test_class').find('.ukuni_application_id').val();
            var clientid = $('#client-id').val();
            var test_url = Drupal.settings.url + '/agent_status_change';
            show_global_loading_overlay();
            $.ajax({
                url: test_url,
                type: 'POST',
                dataType: "json",
                data: {'status_value': status_value, 'ukuni_application_id': ukuni_application_id, 'clientid': clientid},
                success: function (result) {
                    th.closest('.test_class').find('.status-drop').text(result[1]);
                    $('#' + parentvalue).text('');
                    $('#' + parentvalue).html(result[2]);
                    $('#global-overlay').hide();
                    $('#global-overlay').remove();
                    if (result[0] == 1) {
                        Materialize.toast("<ul><li>Status updated successfully</li></ul>", 5000, 'toast alert-success');
                    } else if (result[0] == 0) {
                        Materialize.toast("<ul><li>Fail to update Status of application</li></ul>", 5000, 'toast alert-error');
                    }
                },
            });
            return false;
        });
    });
})(jQuery);

function updateTextInput(val) {
    jQuery("#range_value_fee").val(val);
    jQuery("#range_value_fee_display").text(val);
}


