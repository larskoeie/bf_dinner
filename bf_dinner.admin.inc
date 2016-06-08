<?php


/**
 *
 * Front page of the dinner assigning admin system.
 */
function bf_dinner_admin_front() {
	$assigner = new bf_dinner_auto_assigner();
	$assigner->loadInputData();
	$assigner->makeRandomStatuses();
	return array(
		'create' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '1. Opret',
				'#path' => 'admin/dinner/create',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3 class="' . bf_dinner_get_next_step_class('create') . '"">',
				'#suffix' => '</h3>'
			),
			'p1' => array(
				'#type' => 'markup',
				'#markup' => 'Her oprettes fællesspisninger for den kommende periode. Det er disse, der senere bliver til checkbokse på kan-listen'
			),
			'p2' => array(
				'#theme' => 'item_list',
				'#items' => array(
					'Antal ikke-publicerede fællesspisninger : ' . sizeof(bf_dinner_utility::getAllUnpublishedNodes()),
					'Sidste publicerede fællesspisning : ' . bf_dinner_utility::getLastPublishedDate()->format('d.m.Y'),
					'Antal kokke : ' . bf_dinner_utility::getUserCount(),
					'Næste kan-liste anbefales derfor : ' . bf_dinner_utility::getNextSeriesFirstDateSuggestion()->format('d.m.Y') . ' - ' . bf_dinner_utility::getNextSeriesLastDateSuggestion()->format('d.m.Y') . ' (begge inkl.)',
				),
				'#prefix' => '<div class="stats-info">',
				'#suffix' => '</div>',
			),
			'#prefix' => '<div class="section">',
			'#suffix' => '</div>',

		),
		'edit' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '2. Rediger',
				'#path' => 'admin/dinner/distribute',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3 class="' . bf_dinner_get_next_step_class('edit') . '"">',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'intro' => array(

					'#type' => 'markup',
					'#markup' => 'Efter fællesspisningerne er oprettet, kan de redigeres til at passe til den specifikke periode. Dvs.:'
				),
				'list' => array(
					'#theme' => 'item_list',
					'#items' => array(
						'aflyse fællesspisninger, f.eks. hvis fælleshuset er optaget',
						'ændre antallet af kokke fra en til to, f.eks. i forbindelse med arbejdsweekend',
						'sætte kokke på fællesspisninger manuelt',
					),
					'#type' => 'ul',
				)
			),
			'list' => array(
				'#prefix' => '<div class="stats-info">',
				'#suffix' => '</div>',

			),
			'#prefix' => '<div class="section">',
			'#suffix' => '</div>',

		),
		'open' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '3. Åben kan-lister',
				'#path' => 'admin/dinner/settings',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3 class="' . bf_dinner_get_next_step_class('open') . '"">',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'intro' => array(
					'#type' => 'markup',
					'#markup' => 'Åben for kan-listerne'
				),
				'list' => array(
					'#theme' => 'item_list',
					'#items' => array(
						'Antal brugere',
						'Antal data',
						'Fællesspisninger, som er besat på forhånd',
					),
					'#prefix' => '<div class="stats-info">',
					'#suffix' => '</div>',

				)
			),
			'#prefix' => '<div class="section">',
			'#suffix' => '</div>',

		),
		'close' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '4. Luk kan-lister',
				'#path' => 'admin/dinner/settings',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3 class="' . bf_dinner_get_next_step_class('close') . '"">',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'intro' => array(

					'#type' => 'markup',
					'#markup' => 'Luk for kan-lister.'
				),
			),
			'list' => array(
				'#theme' => 'item_list',
				'#items' => array(
					'Antal udfyldte kan-lister',
					'Gennemsnitligt antal K`er pr. udfyldt kan-liste',
				),
				'#prefix' => '<div class="stats-info">',
				'#suffix' => '</div>',
			),
			'#prefix' => '<div class="section">',
			'#suffix' => '</div>',


		),

		'assign' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '5. Lav fordeling',
				'#path' => 'admin/dinner/assign',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3>',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'intro' => array(
					'#type' => 'markup',
					'#markup' => 'Fællesspisningerne fordeles mellem kokkene, efter følgende regler:'
				),
				'list1' => array(
					'#theme' => 'item_list',
					'#items' => array(
						'alle kokke får tildelt en eller to dage, efter en af følgende tre muligheder:',
						'to dage sammen med en anden ELLER en dag alene ELLER en dag sammen med en anden, hvis dagen er markeret med "2 kokke"',
						'alle kokke får tildelt dag(e), hvor de har markeret at de kan',
					),

				),

				'stat' => array(
					'#theme' => 'item_list',
					'#items' => array(
						'Antal dage der skal fordeles : ' . $assigner->getDateCount(),
						'Antal kokke : ' . $assigner->getUserCount(),
					),
					'#prefix' => '<div class="stats-info">',
					'#suffix' => '</div>',
				),


			),
			'#prefix' => '<div class="section">',
			'#suffix' => '</div>',

		),
		'review' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '6. Gennemse',
				'#path' => 'admin/dinner/review',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3>',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'#type' => 'markup',
				'#markup' => 'Gennemse fordelingen af fællesspisninger for at kontrollere at alt passer.'
			),
			'#prefix' => '<div class="section">',
			'#suffix' => '</div>',


		),
		'publish' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '7. Publicer',
				'#path' => 'admin/dinner/publish',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3>',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'#type' => 'markup',
				'#markup' => 'Publicer alle fællesspisninger, så alle kan se resultatet.'
			),
			'list' => array(
				'#theme' => 'item_list',
				'#prefix' => '<div class="stats-info">',
				'#suffix' => '</div>',

			),
			'#prefix' => '<div class="section">',
			'#suffix' => '</div>',

		),

		'assignments' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => 'Oversigt',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3>',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'#type' => 'markup',
				'#markup' => $assigner->getAssignmentHtml()
			),

		)

	);
}

/**
 *
 */
function bf_dinner_admin_create() {

	$form['info'] = array(

		'#type' => 'markup',
		'#markup' => 'Opret næste kan-liste bestående af et antal fællesspisninger. Vælg første og sidste dato for kan-listen.'
	);

	$form['bf_dinner_start_date'] = array(
		'#type' => 'date_popup',
		'#title' => 'Første fællesspisning',
		'#date_format' => 'd-m-Y',
		'#default_value' => bf_dinner_utility::getNextSeriesFirstDateSuggestion()->format('Y-m-d'),
	);

	$form['bf_dinner_end_date'] = array(
		'#type' => 'date_popup',
		'#title' => 'Sidste fællesspisning',
		'#date_format' => 'd-m-Y',
		'#default_value' => bf_dinner_utility::getNextSeriesLastDateSuggestion()->format('Y-m-d'),
	);

	$form['actions']['submit'] = array(
		'#title' => 'Opret',
		'#type' => 'submit',
	);

	$form['#submit'][] = 'bf_dinner_admin_create_submit';

	return $form;

}

/**
 *
 */
function bf_dinner_admin_edit() {

	// not sure what goes here ...

}

/**
 *
 */
function bf_dinner_admin_assign() {

	$assigner = new bf_dinner_auto_assigner();
	$assigner->loadInputData();
	$assigner->makeRandomStatuses();
	$assigner->validateInput();
	$assigner->assignHalfLoads();
	echo $assigner->getAssignmentHtml();
	die;
	$assigner->assignSingleLoads();
	echo $assigner->getAssignmentHtml();
	$form['info'] = array(

		'#type' => 'markup',
		'#markup' => 'Opret næste kan-liste bestående af et antal fællesspisninger. Vælg første og sidste dato for kan-listen.'
	);

	$form['bf_dinner_start_date'] = array(
		'#type' => 'date_popup',
		'#title' => 'Første fællesspisning',
		'#date_format' => 'd-m-Y',
		'#default_value' => bf_dinner_utility::getNextSeriesFirstDateSuggestion()->format('Y-m-d'),
	);

	$form['bf_dinner_end_date'] = array(
		'#type' => 'date_popup',
		'#title' => 'Sidste fællesspisning',
		'#date_format' => 'd-m-Y',
		'#default_value' => bf_dinner_utility::getNextSeriesLastDateSuggestion()->format('Y-m-d'),
	);

	$form['actions']['submit'] = array(
		'#title' => 'Opret',
		'#type' => 'submit',
	);

	$form['#submit'][] = 'bf_dinner_admin_create_submit';

	return $form;

}


function bf_dinner_admin_create_submit($form, $form_state) {

	$values = $form_state['values'];

	$start_date = new \DateTime($values['bf_dinner_start_date']);
	$end_date = new \DateTime($values['bf_dinner_end_date']);
	$date = $start_date;
	$counter = 0;
	while ($date <= $end_date && $counter++ < 360) {
		bf_dinner_create_node_with_date($date);
		$date = $date->add(new \DateInterval('P1D'));
	}
	drupal_set_message(sprintf('Der blev oprettet %d fællesspisninger.', $counter));
}

/**
 * @param $date \DateTime
 */
function bf_dinner_create_node_with_date($date) {
	global $user;

	$node = new stdClass();
	$node->uid = $user->uid;
	$node->type = 'dinner';
	$node->title = 'Fællesspisning ' . $date->format('d-m-y');
	$node->status = 0;
	$node->field_date['und'][0] = array(
		'value' => $date->format('Y-m-d 00:00:00')
	);

	node_save($node);
}

/**
 *
 */
function bf_dinner_admin_distribute() {

}

/**
 *
 */
function bf_dinner_admin_publish() {

}


/**
 * @param $form
 * @return mixed
 */
function bf_dinner_admin_settings_form($form) {

	$form['bf_dinner_open'] = array(
		'#type' => 'select',
		'#title' => 'Kan-listerne er lige nu',
		'#options' => array(
			0 => t('Lukkede'),
			1 => t('Åbne'),
		),
		'#default_value' => variable_get('bf_dinner_open'),
	);

	$form['bf_dinner_next_deadline'] = array(
		'#type' => 'date_popup',
		'#title' => 'Deadline for denne kan-liste',
		'#date_format' => 'd-m-Y',
		'#default_value' => variable_get('bf_dinner_next_deadline'),
	);

	$form['bf_dinner_header'] = array(
		'#type' => 'textarea',
		'#title' => 'Tekst i toppen af kan-listen',
		'#default_value' => variable_get('bf_dinner_header'),
	);

	return system_settings_form($form);

}

/**
 * @param $step string
 * @return string
 */
function bf_dinner_get_next_step_class($step) {
	$nextStepSuggestion = bf_dinner_utility::getNextStepSuggestion();
	return $step == $nextStepSuggestion ?
		'suggested-next-step' : '';
}
