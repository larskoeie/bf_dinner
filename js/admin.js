/**
 * Created by larskoie on 18/08/16.
 */


jQuery(document).ready(function () {
	jQuery('.interactive td.status').click(function(){
		var td = jQuery(this);
		var div = td.find('div');
		div.text('.');

		var assigned = td.hasClass('assigned-1');
		var newAssigned = ! assigned;
		jQuery.ajax({
			url: '/admin/dinner_assignment_ajax/' + td.data('uid') + '/' + td.data('date') + '/' + (newAssigned ? '1' : '0'),
			success: function(data){
				if (data.assigned || false) {
					td.removeClass('assigned-0').addClass('assigned-1');
					div.text('X');
				} else {
					td.removeClass('assigned-1').addClass('assigned-0');
					div.text('');
				}
				updateOkClasses();
			}
		});

	});

	updateOkClasses();

});

function updateOkClasses () {
	jQuery('th.user').each(function(){
		var uid = jQuery(this).data('uid');
		var userPoints = 0;
		jQuery('.user-' + uid + '.assigned-1').each(function(){
			userPoints += parseInt(config.users[uid].pointsprdate);
		});
//		console.log('userpoints:' + userPoints + ', nominal:' + config.users[uid].pointstotal);
		if (userPoints == config.users[uid].pointstotal) {
			jQuery('.user-' + uid).addClass('user-ok');
		} else {
			jQuery('.user-' + uid).removeClass('user-ok');
		}
	});

	jQuery('th.date').each(function() {
		var date = jQuery(this).data('date');
		var datePoints = 0;
		jQuery('.date-' + date + '.assigned-1').each(function(){
			var uid = jQuery(this).data('uid');
			datePoints += parseInt(config.users[uid].pointsprdate);
		});
//		console.log('datepoints:' + datePoints + ', nominal:' + config.dates[date].pointstotal);
		if (datePoints == config.dates[date].pointstotal) {
			jQuery('.date-' + date).addClass('date-ok');
		} else {
			jQuery('.date-' + date).removeClass('date-ok');
		}
	});
}