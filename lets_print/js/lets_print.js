/* Create new function for printing content of page */
function PrintMe() {
    
	  var style = Drupal.settings.lets_print.printcss;
    window.frames.print_frame.document.body.innerHTML = document.getElementById('main-wrapper').innerHTML;
    var sheet = window.frames.print_frame.document.createElement('style');    
    sheet.innerHTML = style;
    window.frames.print_frame.document.body.appendChild(sheet);
    window.frames.print_frame.window.focus();
    window.frames.print_frame.window.print();

}
