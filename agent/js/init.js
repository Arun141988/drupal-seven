(function ($) {
    $(function () {
        var window_width = $(window).width();
        // convert rgb to hex value string
        function rgb2hex(rgb) {
            if (/^#[0-9A-F]{6}$/i.test(rgb)) {
                return rgb;
            }

            rgb = rgb.match(/^rgb\((\d+),\s*(\d+),\s*(\d+)\)$/);

            if (rgb === null) {
                return "N/A";
            }

            function hex(x) {
                return ("0" + parseInt(x).toString(16)).slice(-2);
            }

            return "#" + hex(rgb[1]) + hex(rgb[2]) + hex(rgb[3]);
        }
        $('.dynamic-color .col').each(function () {
            $(this).children().each(function () {
                var color = $(this).css('background-color'),
                        classes = $(this).attr('class');
                $(this).html(rgb2hex(color) + " " + classes);
                if (classes.indexOf("darken") >= 0 || $(this).hasClass('black')) {
                    $(this).css('color', 'rgba(255,255,255,.9');
                }
            });
        });
        // Floating-Fixed table of contents
        if ($('nav').length) {
            $('.toc-wrapper').pushpin({top: $('nav').height()});
        }
        else if ($('#index-banner').length) {
            $('.toc-wrapper').pushpin({top: $('#index-banner').height()});
        }
        else {
            jq214('.toc-wrapper').pushpin({top: 0});
        }
        // Toggle Flow Text
        var toggleFlowTextButton = $('#flow-toggle');
        toggleFlowTextButton.click(function () {
            $('#flow-text-demo').children('p').each(function () {
                $(this).toggleClass('flow-text');
            });
        });
//    Toggle Containers on page
        var toggleContainersButton = $('#container-toggle-button');
        toggleContainersButton.click(function () {
            $('body .browser-window .container, .had-container').each(function () {
                $(this).toggleClass('had-container');
                $(this).toggleClass('container');
                if ($(this).hasClass('container')) {
                    toggleContainersButton.text("Turn off Containers");
                }
                else {
                    toggleContainersButton.text("Turn on Containers");
                }
            });
        });

        // Detect touch screen and enable scrollbar if necessary
        function is_touch_device() {
            try {
                document.createEvent("TouchEvent");
                return true;
            } catch (e) {
                return false;
            }
        }
        if (is_touch_device()) {
            $('#nav-mobile').css({overflow: 'auto'});
        }

        // Set checkbox on forms.html to indeterminate
        var indeterminateCheckbox = document.getElementById('indeterminate-checkbox');
        if (indeterminateCheckbox !== null)
            indeterminateCheckbox.indeterminate = true;
        // Plugin initialization
        var today = new Date();
        var curyear = today.getFullYear();
        var Yr = curyear - 15;
        $('.datepicker').pickadate(
                {
                    selectYears: true,
                    selectMonths: true,
                    min: new Date(1970, 1, 1),
                    max: new Date(Yr, 1, 1),
                    selectYears: 100,
                            formatSubmit: 'dd/mm/yyyy',
                    format: 'dd/mm/yyyy',
                    hiddenName: true,
                    closeOnSelect: true,
                    closeOnClear: false,
                }
        );
        $('.passportdatepicker').pickadate(
                {
                    selectYears: true,
                    selectMonths: true,
                    min: new Date(),
                    selectYears: 10,
                            formatSubmit: 'dd/mm/yyyy',
                    format: 'dd/mm/yyyy',
                    hiddenName: true,
                    closeOnSelect: true,
                    closeOnClear: false,
                }
        );
        $('select').not('.disabled').material_select();
        $('.modal-trigger').leanModal({
            dismissible: true, // Modal can be dismissed by clicking outside of the modal
            opacity: .1, // Opacity of modal background
            in_duration: 300, // Transition in duration
            out_duration: 200, // Transition out duration
            ready: function () {
            }, // Callback for Modal open
            complete: function () {
                $('.lean-overlay').remove();
            } // Callback for Modal close
        });
    }); // end of document ready
})(jq214); // end of jQuery name space


// Use latest jQuery
jq214(function ($) {
    $(document).ready(function () {
        var k, institution_id = '', course_id = '', courseAutocmpleteElemetid = '', courseHiddenfdlID = '';
        for (k = 0; k < 10; k++) {
            institution_id = $('#institution-' + k).val();
            course_id = $('#course-' + k).val();
            if (parseInt(institution_id) > 0) {
                courseAutocmpleteElemetid = "edit-courses-" + k + "-course";
                courseHiddenfdlID = "course-" + k;
                setCourseAjax(institution_id, courseAutocmpleteElemetid, course_id, courseHiddenfdlID);
            }
        }

        var institution_id = $('#institution').val();
        var course_id = $('#course').val();
        if (parseInt(institution_id) > 0) {
            jq214(".main_add").show()
            jq214(".main_add").trigger("click");
            courseAutocmpleteElemetid = "course_dropdown";
            courseHiddenfdlID = "course";
            setCourseAjax(institution_id, courseAutocmpleteElemetid, course_id, courseHiddenfdlID);
        }

        function setCourseAjax(institution_id, courseAutocmpleteElemetid, course_id, courseHiddenfdlID) {
            $.ajax({
                dataType: "json",
                type: 'Get',
                url: '/application/applications_insi1_course_ajax',
                data: {uni_id: institution_id},
                success: function (coursedata) {
                    if (coursedata == 0) {
                        jQuery("#" + courseAutocmpleteElemetid).attr("disabled", "disabled");
                        jQuery("#" + courseAutocmpleteElemetid).val("No Course Found");
                    } else {
                        jQuery("#" + courseAutocmpleteElemetid).removeAttr("disabled");
                        if (parseInt(course_id) < 0 || course_id == '') {
                            jQuery("#" + courseAutocmpleteElemetid).val("Select Course");
                        }
                    }
                    jQuery("#global-overlay").hide();
                    jQuery("#global-overlay").remove();
                    jQuery("#" + courseAutocmpleteElemetid).autoComplete({
                        minChars: 0,
                        source: function (term, suggest) {
                            term = term.toLowerCase();
                            var choices = coursedata;
                            var suggestions = [];
                            for (i = 0; i < choices.length; i++)
                                if (~(choices[i][0] + ' ' + choices[i][1]).toLowerCase().indexOf(term))
                                    suggestions.push(choices[i]);
                            suggest(suggestions);
                        },
                        renderItem: function (item, search) {
                            search = search.replace(/[-\/\\^$*+?.()|[\]{}]/g, '\\$&');
                            var re = new RegExp("(" + search.split(' ').join('|') + ")", "gi");
                            return '<div class="autocomplete-suggestion" data-langname="' + item[0] + '" data-lang="' + item[1] + '" data-val="' + search + '"> ' + item[0].replace(re, "<b>$1</b>") + '</div>';
                        },
                        onSelect: function (e, term, item) {
                            $("#" + courseAutocmpleteElemetid).val(item.data('langname'));
                            $('#' + courseHiddenfdlID).val(item.data('lang'));
                        }
                    });
                },
                error: function (data) {
                }
            });
        }


        //START AUTOCOMPLETE
        jQuery("#uni_dropdown,#course_dropdown, .nationality_dropdown, .countries_dropdown").keydown(function (event) {
            if (event.keyCode == 13) {
                event.preventDefault();
                return false;
            }
        });
        jQuery('#uni_dropdown').focus(function () {
            jQuery(this).val('');
            jQuery("#institution").val('');
            jQuery("#course").val('');
        });
        jQuery('#course_dropdown').focus(function () {
            jQuery(this).val('');
            jQuery("#course").val('');
        });
        jQuery('#uni_dropdown').blur(function () {
            var val = jQuery(this).val();
            if (val == '') {
                jQuery(this).val('Select Institutions');
            }

        });
        jQuery('#course_dropdown').blur(function () {
            var val = jQuery(this).val();
            if (val == '') {
                jQuery(this).val('Select Course');
            }
        });
        jQuery('#uni_dropdown').autoComplete({
            minChars: 0,
            source: function (term, suggest) {
                var test_url = Drupal.settings.url + '/uni_list';
                $.ajax({
                    dataType: "json",
                    type: 'Get',
                    url: test_url,
                    success: function (data) {
                        term = term.toLowerCase();
                        var choices = data;
                        var suggestions = [];
                        for (i = 0; i < choices.length; i++)
                            if (~(choices[i][0] + ' ' + choices[i][1]).toLowerCase().indexOf(term))
                                suggestions.push(choices[i]);
                        suggest(suggestions);
                    },
                    error: function (data) {
                    }
                });
            },
            renderItem: function (item, search) {
                search = search.replace(/[-\/\\^$*+?.()|[\]{}]/g, '\\$&');
                var re = new RegExp("(" + search.split(' ').join('|') + ")", "gi");
                return '<div class="autocomplete-suggestion" data-langname="' + item[0] + '" data-lang="' + item[1] + '" data-val="' + search + '"> ' + item[0].replace(re, "<b>$1</b>") + '</div>';
            },
            onSelect: function (e, term, item) {
                show_global_loading_overlay();
                $('#uni_dropdown').val(item.data('langname'));
                $('#institution').val(item.data('lang'));
                //another ajax to fetch course uni wise
                $.ajax({
                    dataType: "json",
                    type: 'Get',
                    url: '/application/applications_insi1_course_ajax',
                    data: {uni_id: item.data('lang')},
                    success: function (coursedata) {
                        if (coursedata == 0) {
                            jQuery("#course_dropdown").attr("disabled", "disabled");
                            jQuery("#course_dropdown").val("No Course Found");
                        } else {
                            jQuery("#course_dropdown").removeAttr("disabled");
                            jQuery("#course_dropdown").val("Select Course");
                        }


                        jQuery("#global-overlay").hide();
                        jQuery("#global-overlay").remove();
                        jQuery('#course_dropdown').autoComplete({
                            minChars: 0,
                            source: function (term, suggest) {
                                term = term.toLowerCase();
                                var choices = coursedata;
                                var suggestions = [];
                                for (i = 0; i < choices.length; i++)
                                    if (~(choices[i][0] + ' ' + choices[i][1]).toLowerCase().indexOf(term))
                                        suggestions.push(choices[i]);
                                suggest(suggestions);
                            },
                            renderItem: function (item, search) {
                                search = search.replace(/[-\/\\^$*+?.()|[\]{}]/g, '\\$&');
                                var re = new RegExp("(" + search.split(' ').join('|') + ")", "gi");
                                return '<div class="autocomplete-suggestion" data-langname="' + item[0] + '" data-lang="' + item[1] + '" data-val="' + search + '"> ' + item[0].replace(re, "<b>$1</b>") + '</div>';
                            },
                            onSelect: function (e, term, item) {
                                $('#course_dropdown').val(item.data('langname'));
                                $('#course').val(item.data('lang'));
                            }
                        });
                    },
                    error: function (data) {
                    }
                });
            }
        });
        //END AUTOCOMPLETE

        jQuery('.institutions-draft').live('focus', function () {
            var ElemeteId = '';
            ElemeteId = jQuery(this).attr('id');
            jQuery("#" + ElemeteId).val('');
            var appIndexNum = ElemeteId.match(/\d+/);
            jQuery("#institution-" + appIndexNum).val('');
            jQuery("#course-" + appIndexNum).val('');
            var courseElemeteId = "edit-courses-" + appIndexNum + "-course";
            jQuery("#" + courseElemeteId).val('Select Course');
            /*Function use for load derft application institutions & course values*/
            loadAutoCompleteinstcourse(appIndexNum, ElemeteId, courseElemeteId);
        });

        jQuery('.institutions-draft').blur(function () {
            var ElemeteId = '';
            ElemeteId = jQuery(this).attr('id');
            var val = jQuery("#" + ElemeteId).val();
            if (val == '') {
                jQuery("#" + ElemeteId).val('Select Institutions');
            }
        });

        jQuery('.courses-draft').live('focus', function () {
            var courseElemeteId = '';
            courseElemeteId = jQuery(this).attr('id');
            var appCourseIndexNum = courseElemeteId.match(/\d+/);
            jQuery("#" + courseElemeteId).val('');
            jQuery("#course-" + appCourseIndexNum).val('');
        });

        jQuery('.courses-draft').blur(function () {
            var courseElemeteId = '';
            courseElemeteId = jQuery(this).attr('id');
            val = jQuery("#" + courseElemeteId).val();
            if (val == '') {
                jQuery("#" + courseElemeteId).val('Select Course');
            }
        });

        function loadAutoCompleteinstcourse(appIndexNum, instElemeteId, courseElemeteId) {
            jQuery('#' + instElemeteId).autoComplete({
                // jQuery('#edit-courses-0-institution').autoComplete({            
                minChars: 0,
                source: function (term, suggest) {
                    var test_url = Drupal.settings.url + '/uni_list';
                    $.ajax({
                        dataType: "json",
                        type: 'Get',
                        url: test_url,
                        success: function (data) {
                            term = term.toLowerCase();
                            var choices = data;
                            var suggestions = [];
                            for (i = 0; i < choices.length; i++)
                                if (~(choices[i][0] + ' ' + choices[i][1]).toLowerCase().indexOf(term))
                                    suggestions.push(choices[i]);
                            suggest(suggestions);
                        },
                        error: function (data) {
                        }
                    });
                },
                renderItem: function (item, search) {
                    search = search.replace(/[-\/\\^$*+?.()|[\]{}]/g, '\\$&');
                    var re = new RegExp("(" + search.split(' ').join('|') + ")", "gi");
                    return '<div class="autocomplete-suggestion" data-langname="' + item[0] + '" data-lang="' + item[1] + '" data-val="' + search + '"> ' + item[0].replace(re, "<b>$1</b>") + '</div>';
                },
                onSelect: function (e, term, item) {
                    show_global_loading_overlay();
                    var univeri_id = '';
                    $('#' + instElemeteId).val(item.data('langname'));
                    $('#institution-' + appIndexNum).val(item.data('lang'));
                    univeri_id = item.data('lang');
                    //another ajax to fetch course uni wise
                    $.ajax({
                        dataType: "json",
                        type: 'Get',
                        url: '/application/applications_insi1_course_ajax',
                        data: {uni_id: univeri_id},
                        success: function (coursedata) {
                            if (coursedata == 0) {
                                jQuery("#" + courseElemeteId).attr("disabled", "disabled");
                                jQuery("#" + courseElemeteId).val("No Course Found");
                            } else {
                                jQuery("#" + courseElemeteId).removeAttr("disabled");
                                jQuery("#" + courseElemeteId).val("Select Course");
                            }
                            jQuery("#global-overlay").hide();
                            jQuery("#global-overlay").remove();
                            jQuery('#' + courseElemeteId).autoComplete({
                                minChars: 0,
                                source: function (term, suggest) {
                                    term = term.toLowerCase();
                                    var choices = coursedata;
                                    var suggestions = [];
                                    for (i = 0; i < choices.length; i++)
                                        if (~(choices[i][0] + ' ' + choices[i][1]).toLowerCase().indexOf(term))
                                            suggestions.push(choices[i]);
                                    suggest(suggestions);
                                },
                                renderItem: function (item, search) {
                                    search = search.replace(/[-\/\\^$*+?.()|[\]{}]/g, '\\$&');
                                    var re = new RegExp("(" + search.split(' ').join('|') + ")", "gi");
                                    return '<div class="autocomplete-suggestion" data-langname="' + item[0] + '" data-lang="' + item[1] + '" data-val="' + search + '"> ' + item[0].replace(re, "<b>$1</b>") + '</div>';
                                },
                                onSelect: function (e, term, item) {
                                    $('#' + courseElemeteId).val(item.data('langname'));
                                    $('#course-' + appIndexNum).val(item.data('lang'));
                                }
                            });
                        },
                        error: function (data) {
                        }
                    });
                }
            });
        }


        //START NATIONALITY DROPDOWN
        jQuery('.nationality_dropdown').autoComplete({
            minChars: 0,
            source: function (term, suggest) {
                var test_url = Drupal.settings.url + '/nationality_list';

                $.ajax({
                    dataType: "json",
                    type: 'Get',
                    url: test_url,
                    success: function (data) {
                        term = term.toLowerCase();
                        var choices = data;
                        var suggestions = [];
                        for (i = 0; i < choices.length; i++)
                            if (~(choices[i][0] + ' ' + choices[i][1]).toLowerCase().indexOf(term))
                                suggestions.push(choices[i]);
                        suggest(suggestions);
                    },
                    error: function (data) {

                    }
                });
            },
            renderItem: function (item, search) {
                search = search.replace(/[-\/\\^$*+?.()|[\]{}]/g, '\\$&');
                var re = new RegExp("(" + search.split(' ').join('|') + ")", "gi");
                return '<div class="autocomplete-suggestion" data-langname="' + item[0] + '" data-lang="' + item[1] + '" data-val="' + search + '"> ' + item[0].replace(re, "<b>$1</b>") + '</div>';
            },
            onSelect: function (e, term, item) {
                $('.nationality_dropdown').val(item.data('langname'));
            },
        });
        //END NATIONALITY DROPDOWN

        //START COUNTRY DROPDOWN
        jQuery('.countries_dropdown').autoComplete({
            minChars: 0,
            source: function (term, suggest) {
                var test_url = Drupal.settings.url + '/country_list';

                $.ajax({
                    dataType: "json",
                    type: 'Get',
                    url: test_url,
                    success: function (data) {
                        term = term.toLowerCase();
                        var choices = data;
                        var suggestions = [];
                        for (i = 0; i < choices.length; i++)
                            if (~(choices[i][0] + ' ' + choices[i][1]).toLowerCase().indexOf(term))
                                suggestions.push(choices[i]);
                        suggest(suggestions);
                    },
                    error: function (data) {

                    }
                });
            },
            renderItem: function (item, search) {
                search = search.replace(/[-\/\\^$*+?.()|[\]{}]/g, '\\$&');
                var re = new RegExp("(" + search.split(' ').join('|') + ")", "gi");
                return '<div class="autocomplete-suggestion" data-langname="' + item[0] + '" data-lang="' + item[1] + '" data-val="' + search + '"> ' + item[0].replace(re, "<b>$1</b>") + '</div>';
            },
            onSelect: function (e, term, item) {
                $('.countries_dropdown').val(item.data('langname'));
            },
        });
        //END COUNTRY DROPDOWN


        $(".select-wrapper ul li").click(function () {
            $(this).parent().css('display', 'none');
        });
    });

    /*Code for Open Popup before add new application*/
    var saveBtnId = '';
    var appSaveBtnIndexNum = '';
    $('.modal-op').on('click', function (event) {
        var institute = '';
        var courses = '';
        var application_id = '';
        saveBtnId = $(this).attr('id');
        appSaveBtnIndexNum = saveBtnId.match(/\d+/);
        application_id = $("#application-id-" + appSaveBtnIndexNum).val();
        if (parseInt(application_id) > 0) {
            institute = $("#institution-" + appSaveBtnIndexNum).val();
            courses = $("#course-" + appSaveBtnIndexNum).val();
        } else {
            institute = $("#institution").val();
            courses = $("#course").val();
        }
        var intake = $("#app-intake-" + appSaveBtnIndexNum).val();
        var entryyear = $("#app-entry-year-" + appSaveBtnIndexNum).val();
        var presessional = $("#app-pre-sessional-" + appSaveBtnIndexNum).val();
        if ((institute.trim() != '' || parseInt(institute) > 0) && (courses.trim() != '' || parseInt(courses) > 0) && intake.trim() != ''
                && entryyear.trim() != '' && presessional.trim() != '') {
            $("#declaration-modal").trigger("click");
        } else {
            $("#app-required").trigger("click");
        }
    });
    $(".apply-app").on('click', function (event) {
        $("#edit-courses-" + appSaveBtnIndexNum + "-savenew").trigger("click");
    });

    /*Confirmation modal Box while delete document*/
    $(".open-del-conf-modal").on('click', function (event) {
        var DeleteBtnObj = $(this);
        var DeleteDocId = DeleteBtnObj.attr('rel');
        var DeleteFileName = $(this).closest('td').siblings(':first-child').text();
        $('#delfileName').text(DeleteFileName);
        $('#delete-file').attr("rel", DeleteDocId);
    });
    $("#delete-file").on('click', function (event) {
        var DeleteBtnObj = $(this);
        var DeleteDocId = DeleteBtnObj.attr('rel');
        $("#edit-document-delete-" + DeleteDocId).trigger("click");
    });
});
