/**
 *
 * JavaScript for dinner form.
 *
 */
jQuery(document).ready(function () {

	// copy some classes to parent objects for later use
	jQuery('.user-self').parent().addClass('user-self');
	jQuery('.user-other').parent().addClass('user-other');

	jQuery('.weekend').parents('.form-item').addClass('weekend');

	// when self-or-all-dropdown changes value :
	// - show or hide some form elements accordingly
	// - change title of other elements accordingly
	jQuery('#edit-show-other-users').change(function () {
		if (jQuery(this).val() == 1) {
			jQuery('.shortcut, .user-other').show();
		} else {
			jQuery('.shortcut, .user-other').hide();
		}
	}).change();

	// iterate through dates shown in form
	jQuery('.form-type-checkboxes').each(function (i) {
		var e = jQuery(this);
		// iterate through checkboxes related to this date
		e.find('.form-type-checkbox').each(function (j) {
			// the first checkbox is the shortcut that should change all of the other checkboxes
			if (j == 0) {
				jQuery(this).addClass('shortcut');
				jQuery(this).find('input').click(function () {
					console.log(jQuery(this).attr('checked'));
					if (jQuery(this).attr('checked')) {
						e.find('.user-self,.user-other').find('input').attr('checked', 'checked');
					} else {
						e.find('.user-self,.user-other').find('input').attr('checked', '');
					}

				});
			} else {
				// when changing a checkbox that is not a shortcut, check the corresponding shortcut checkbox if ALL other checkboxes are checked
				e.find('input').click(function(){
					e.find('.shortcut input').attr('checked', e.find('.form-item:not(.shortcut) input:visible:not(:checked)').size() ? '' : 'checked');
				});
			}
			// the second checkbox is the user-self, so add that class
			if (j == 1) {
				jQuery(this).addClass('user-self');
			}
			// all remaining checkboxes are user-other, so add that class
			if (j > 1) {
				jQuery(this).addClass('user-other');
			}
		});

	});

	// if user touches a checkbox related to another user, lock form, making it impossible to switch to user-self-only mode
	jQuery('.user-other input').click(function () {
		jQuery('.form-item-show-other-users select').attr('disabled', 'disabled');
	});

});