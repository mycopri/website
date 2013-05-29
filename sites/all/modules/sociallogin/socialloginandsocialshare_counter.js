var selectedCounterTheme = document.getElementsByName('socialloginandsocialshare_selected_counter_interface');
window.onload = function(){
loginRadiusToggleCounterTheme(selectedCounterTheme[0].value);
    jQuery("#socialloginandsocialshare_counter_horizontal").click(function(){
    loginRadiusToggleCounterTheme("horizontal");
});
  jQuery("#socialloginandsocialshare_counter_veritical").click(function(){															
  loginRadiusToggleCounterTheme("vertical");
});
}
function loginRadiusToggleCounterTheme(theme){
  selectedCounterTheme[0].value=theme;
  if(theme == "vertical"){
    verticalDisplay = 'block';
    horizontalDisplay = 'none';
	marginleft ='78px';
	jQuery(".form-item.form-type-radios.form-item-socialshare-counter-vertical-position").removeClass("element-invisible");
   }else{
   verticalDisplay = 'none';
    horizontalDisplay = 'block';
	marginleft ='20px';
    jQuery(".form-item.form-type-radios.form-item-socialshare-counter-vertical-position").addClass("element-invisible");
}
document.getElementById('socialloginandsocialshare_counter_horizontal_images').style.display = horizontalDisplay;
document.getElementById('socialloginandsocialshare_counter_vertical_images').style.display = verticalDisplay;
document.getElementById('lrsharing_divwhite').style.marginLeft  = marginleft;
document.getElementById('lrsharing_divgrey').style.marginLeft  = marginleft;
}
