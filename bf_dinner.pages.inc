<?php


/**
 * Form that allows user to change dinner statuses in db.
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function bf_dinner_user_status_form($form, &$form_state) {
	// add logged in user to array of users in house
	global $user;
	$uid = $user->uid;
	$user = user_load($uid);
	$users = array($uid => bf_dinner_utility::getUserName($uid));
	$usersWithShortcut = array(0 => 'begge', $uid => bf_dinner_utility::getUserName($uid));

	// get house and add other users in house to array
	$uids = array();
	if ($temp = field_get_items('user', $user, DINNER_FIELD_USER_HOUSE)) {
		$house = intval($temp[0]['tid']);
		// get all users attached to house, except the logged in user, which is already added to arrays
		$q = new EntityFieldQuery();
		$temp = $q
			->entityCondition('entity_type', 'user')
			->entityCondition('bundle', 'user')
			->propertyCondition('uid', $uid, '<>')
			->fieldCondition(DINNER_FIELD_USER_HOUSE, 'tid', $house)
			->execute();

		foreach ($temp['user'] as $u => $dummy) {
			$us = user_load($u);
			$users[$u] = bf_dinner_utility::getUserName($u);
			$usersWithShortcut[$u] = bf_dinner_utility::getUserName($u);
			$uids[$u] = $u;
		}

	}
	$form['info'] = array(
		'#markup' => variable_get('bf_dinner_header'),
	);
	$deadline = new \DateTime(variable_get('bf_dinner_next_deadline', ''));
	$form['info2'] = array(
		'#markup' => 'Deadline : ' . $deadline->format('d. m. Y'),
	);

	// the value of this form field is not stored on server - could be stored in cookie
	$form['show_other_users'] = array(
		'#title' => 'Vælg kan-liste(r)',
		'#type' => 'select',
		'#options' => array(
			0 => 'udfyld kun min egen kan-liste',
			1 => 'udfyld alle husstandens kan-lister samtidig',
		),
		'#default_value' => 0,
	);

	// the value of this form field is stored in a field on user's account
	foreach ($users as $u => $name) {
		$form['points-' . $u] = array(
			'#title' => $name . ' vil gerne lave mad',
			'#type' => 'select',

			'#options' => array(
				2 => 'alene',
				1 => 'sammen med en anden',
			),
			'#default_value' => bf_dinner_utility::getUserLoadPrDay($u),
			'#attributes' => array(
				'class' => array($u == $uid ? 'user-self' : 'user-other'),
			)

		);

	}

	$_m = '';
	$nodes = bf_dinner_utility::getAllUnpublishedNodes();
	foreach ($nodes as $node) {
		$classes = array();
		$node = node_load($node->nid);
		$date = field_get_items('node', $node, 'field_date')[0]['value'];
		$date = new \DateTime($date);
		$date_string = $date->format('Y-m-d');

		$dateLoad = bf_dinner_utility::getDateLoad($node);
		$body = field_get_items('node', $node, 'body')[0]['value'];
		// not used pt.
		// $dateState = field_get_items('node', $node, DINNER_FIELD_DATE_STATE)[0]['value'];

		// the title next to the checkbox(es)
		$title = $date->format('l \d. d.');
		$title .= _views_holidays_get_holidays($date);
		if (strlen(trim($body))) {
			$title .= '&nbsp;:&nbsp;' . $body;
		}

		// month label
		if ($date->format('m') != $_m) {
			$form['month-' . $date->format('m')] = array(
				'#type' => 'markup',
				'#markup' => $date->format('F'),
				'#prefix' => '<h2 class="header-month">',
				'#suffix' => '</h2>',
			);
			$_m = $date->format('m');
		}

		// week label
		if ($date->format('W') != $_w) {
			$form['week-' . $date->format('W')] = array(
				'#type' => 'markup',
				'#markup' => $date->format('\U\g\e W'),
				'#prefix' => '<h2 class="header-week">',
				'#suffix' => '</h2>',
			);
			$_w = $date->format('W');
		}

		// weekday
		$isWeekend = $date->format('N') > 5;
		if ($isWeekend) {
			$classes[] = 'weekend';
		}
		if ($dateLoad > 0) {
			// scheduled
			$default = array();
			foreach (bf_dinner_utility::getUserDateStatuses($uids, $date_string, DINNER_USER_DATE_STATUS_POSSIBLE) as $uid => $status) {
				if ($status) {
					$default[$uid] = $uid;
				}
			}

			$form['date-' . $date_string . '-status'] = array(
				'#type' => 'checkboxes',
				'#options' => $usersWithShortcut,
				//'#type' => 'checkbox',
				'#title' => $title,
				'#description' => $body,
				'#default_value' => $default,
				'#attributes' => array(
					'class' => $classes,
				)
			);
			/*
						$form['date-' . $date->format('Y-m-d') . '-kids'] = array(
							'#type' => 'checkbox',
						);
			*/
		} else {
			// not scheduled
			$form['date-' . $date_string . '-status'] = array(
				'#type' => 'checkboxes',
				'#title' => $title,
				'#description' => $body,

				'#options' => array(
					0 => $body
				),
				'#attributes' => array(
					'class' => array('dinner-state-not-scheduled'),
				)
			);

		}
	}

	$form['actions']['submit'] = array(
		'#title' => 'Opret',
		'#type' => 'submit',
	);

	$form['#submit'][] = 'bf_dinner_user_status_form_submit';
	$form['#attached']['js'] = array(
		drupal_get_path('module', 'bf_dinner') . '/js/form.js',
	);

	return $form;

}

/**
 * Submission handler for dinner form. Writes :
 * - dinner load half or full to profile of logged in user and other user(s)
 * - statuses for logged in user and other user(s)
 *
 * @param $form
 * @param $form_state
 *
 */
function bf_dinner_user_status_form_submit($form, &$form_state) {
	$values = $form_state['values'];

	foreach ($values as $field => $value) {

		if (preg_match('/points-([0-9]+)/', $field, $m)) {
			if ($value) {
				bf_dinner_utility::setUserLoadPrDay($m[1], $value);
			}
		}

		if (preg_match('/date-([0-9]+-[0-9]+-[0-9]+)-status/', $field, $m)) {
			foreach ($value as $uid => $val) {
				if ($uid) {
					bf_dinner_utility::setUserDateStatus($uid, $m[1], $val ? 1 : 0);
				}

			}
		}
	}
	drupal_set_message('Dine kan-liste krydser er gemt.');
}