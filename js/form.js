/**
 *
 * JavaScript for dinner form.
 *
 */
var lastConflictState = false;
var allowOtherCheckboxes,
	otherUserName = '';


jQuery(document).ready(function () {

	allowOtherCheckboxes = jQuery('#edit-show-other-users').size() > 0;

	// copy some classes to parent objects for later use
	jQuery('.user-self').parent().addClass('user-self');
	jQuery('.user-other').parent().addClass('user-other');

	jQuery('.weekend').parents('.form-item').addClass('weekend');

	// iterate through dates shown in form
	jQuery('.form-type-checkboxes').each(function (i) {
		var e = jQuery(this);
		// iterate through checkboxes related to this date
		e.find('.form-type-checkbox').each(function (j) {
			// the first checkbox is the shortcut that should change all of the other checkboxes
			if (j == 0) {
				jQuery(this).addClass('shortcut');
				jQuery(this).find('input').click(function () {
					if (jQuery(this).attr('checked')) {
						e.find('.user-self,.user-other').find('input').attr('checked', true);
					} else {
						e.find('.user-self,.user-other').find('input').attr('checked', false);
					}

				});
			} else {
				// when changing a checkbox that is not a shortcut, check the corresponding shortcut checkbox if ALL other checkboxes are checked
				e.find('.shortcut input').attr('checked', !e.find('.form-item:not(.shortcut) input:visible:not(:checked)').size());
				e.find('input').click(function () {
					e.find('.shortcut input').attr('checked', !e.find('.form-item:not(.shortcut) input:visible:not(:checked)').size());
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

	// when self-or-all-dropdown changes value :
	// - show or hide some form elements accordingly
	// - change title of other elements accordingly
	jQuery('#edit-show-other-users').change(function () {
		if (jQuery(this).val() == 1) {
			jQuery('.shortcut, .user-other').slideDown('slow');
		} else {
			jQuery('.shortcut, .user-other').slideUp('slow');
		}
	});

	jQuery('.shortcut, .user-other').hide();

	// if user touches a checkbox related to another user, lock form, making it impossible to switch to user-self-only mode
	jQuery('.user-other input').click(function () {
//		jQuery('.form-item-show-other-users select').attr('disabled', 'disabled');
	});

	jQuery('.form-type-checkboxes:last').addClass('last');

	setInterval(pollConflictRisc, 1000);

});

function pollConflictRisc() {
	// poll ajax url
	jQuery.ajax({
		url: '/dinner_conflict_ajax',
		data: {
			otherUserName: otherUserName
		},
		dataType: 'json',
		success: function (data) {
			console.log(data);
			// risc state has changed ?
			if ((data.risc || false) != lastConflictState) {
				// Risc state changed, we need to show a message. Check that there is a container for it.
				if (! jQuery('#messages').size()) {
					jQuery('#header').after(
						jQuery('<div/>').attr('id', 'messages')
					);
				}
				// remove any old messages
				jQuery('#messages .messages').slideUp('slow');
				// Append the new message
				jQuery('#messages').append(
						jQuery('<div/>').addClass('section clearfix').append(
							jQuery('<div/>').addClass('messages').addClass(data.type).text(data.messageAjax).hide().slideDown('slow')
						)
				);
				// Hide other-user fields and the other-user selector
				jQuery('#edit-show-other-users').val(0).trigger('change');
				jQuery('.form-item-show-other-users').slideUp('slow');
				// Remember risc state for next time
				lastConflictState = data.risc;
				otherUserName = data.otherUserName || '';
			}
		}
	});
}