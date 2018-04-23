<?php


/**
 *
 * Front page of the dinner assigning admin system.
 */
function bf_dinner_admin_front() {
	$assigner = new bf_dinner_auto_assigner();
	$assigner->loadInputData();
	$assigner->validateInput();
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
			'p2' => bf_dinner_admin_info_create(),
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
						'sætte kokke på fællesspisninger manuelt. Bemærk at du kan kun besætte dage fuldt ud, ikke halvt.',
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
						'alle kokke får tildelt dag(e), hvor de har markeret at de kan, dog tæller en ikke-afleveret liste som "Kan alle dage"',
						'hvis kok(ke) er tildelt manuelt til en dag, ignoreres det om de har sat K på kan-listen',
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
		'publish' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '6. Publicer',
				'#path' => 'admin/dinner/publish',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3>',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'#type' => 'markup',
				'#markup' => 'Publicer alle fællesspisninger, så alle kan se resultatet. Bemærk: efter dette er gjort, kan automatisk tildeling ikke køre igen. Kokke kan altid byttes rundt manuelt.'
			),
			'list' => bf_dinner_admin_info_publish(),
			'#prefix' => '<div class="section">',
			'#suffix' => '</div>',

		),

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

    $form['info'] = bf_dinner_admin_info_create();

	$currentSeriesId = bf_dinner_utility::getCurrentSeriesId();
	$currentSeriesId = bf_dinner_utility::getCurrentSeriesId();
	$form['bf_dinner_series_id'] = array(
		'#type' => 'textfield',
		'#title' => 'Næste runde nummer',
		'#description' => sprintf('Sidste runde havde nummer %d, den skal nok forlænges', $currentSeriesId),
		'#default_value' => $currentSeriesId,
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
function bf_dinner_admin_assign($form, $form_state) {

    $assigner = new bf_dinner_auto_assigner();
    $assigner->loadInputData();
    $assigner->validateInput();

    if (! empty($form_state['input']['op'])) {
        switch ($form_state['input']['op']) {
            case 'clear' :
                $assigner->clearTemporaryForSeries(4);
                break;

            case 'Fordel automatisk' :
                $assigner->assignHalfLoads();
                $assigner->assignSingleLoads();
                $assigner->writeToTemporary();
                break;

            case 'Gem fordeling permanent' :
                bf_dinner_utility::writeTemporaryAssignmentsToPermanent();
                break;

        }
    }

    $assigner->setInteractive(TRUE);

	drupal_set_message(implode("\n", $assigner->getMessages()));

    $form['series'] = array(

        '#type' => 'select',
        '#options' => array(
            bf_dinner_utility::getAllSeriesForSelectOptions()
        ),
        '#default_value' => !empty($_POST['series'])
    );

    $form['update'] = array(
        '#type' => 'submit',
        '#value' => 'Opdater',
    );

    $form['table'] = array(
        '#type' => 'markup',
        '#markup' => $assigner->getAssignmentHtml()
    );

    $form['buttons'] = array(
        'clear' => array(
            '#type' => 'button',
            '#value' => t('clear'),
            '#weight' => 10,
        ),

        'auto' => array(
            '#type' => 'button',
            '#value' => t('Fordel automatisk'),
            '#weight' => 20,
        ),

        'writeToFields' => array(
            '#type' => 'button',
            '#value' => t('Gem fordeling permanent'),
            '#weight' => 30,
        ),

    );

    return $form;

}


function bf_dinner_admin_create_submit($form, $form_state) {

	$values = $form_state['values'];

	$series_id = $values['bf_dinner_series_id'];
	$start_date = new \DateTime($values['bf_dinner_start_date']);
	$end_date = new \DateTime($values['bf_dinner_end_date']);
	$date = $start_date;
	$counter = 0;
	while ($date <= $end_date && $counter++ < 360) {
	    if (bf_dinner_utility::getDateNode($date) === FALSE) {
    		bf_dinner_create_node_with_date($date, $series_id);
            $created ++;
        } else {
            $existed ++;
        }
		$date = $date->add(new \DateInterval('P1D'));
	}
	drupal_set_message(sprintf('Der blev oprettet %d fællesspisninger - %d fandtes allerede (evt. som reservationer).', $created, $existed));
}

/**
 * @param $date \DateTime
 */
function bf_dinner_create_node_with_date($date, $series_id) {
	global $user;

	$node = new stdClass();
	$node->uid = $user->uid;
	$node->type = 'dinner';
	$node->title = 'Fællesspisning ' . $date->format('d-m-y');
	$node->status = 0;
	$node->field_date['und'][0] = array(
		'value' => $date->format('Y-m-d 00:00:00'),
	);

    // add date to this other field as well
    if (! empty(DINNER_FIELD_DATE_DATEINTERVAL)) {
        $node->{DINNER_FIELD_DATE_DATEINTERVAL}['und'][0] = array(
            'value' => $date->format('Y-m-d 00:00:00'),
            'value2' => $date->format('Y-m-d 00:00:00'),
        );
    }

    $node->{DINNER_FIELD_DATE_SERIES}['und'][0] = array(
		'value' => $series_id,
	);
	node_save($node);
}

/**
 *
 */
function bf_dinner_admin_publish($form, $form_state) {

    if ($form_state['input']['op'] === 'Publicer') {

        foreach (bf_dinner_utility::getNodesForPublishing() as $date) {

        }
    }

    $form['info'] = bf_dinner_admin_info_publish();

    $form['submit'] = array(
        '#type' => 'button',
        '#value' => 'Publicer'
    );

    return $form;
}


/**
 *
 */
function bf_dinner_admin_users() {
    $header = array(
        'Hus',
        'Navn',
        'Tjanser i kommende runde',
        'Alene/sammen',
    );



    $rows = [];
    /** @var \Drupal\bf_dinner\Entity\User $us */
    foreach (bf_dinner_utility::getAllUsers() as $us) {


        $el = array(
            '#type' => 'select',
            '#options' => array(
                0 => 'Ingen',
                2 => 'En',
                4 => 'To',
            ),
            '#default_value' => 2 //bf_dinner_utility::getUserFieldValue($us->uid, DINNER_FIELD_USER_LOAD_TOTAL),
        );

        $rows[] = array(
            'data' => array(
                bf_dinner_utility::getUserFieldValue($us->uid, DINNER_FIELD_USER_HOUSE),

                l(
                    bf_dinner_utility::getUserName($us->uid),
                    sprintf('user/%d/dinner', $us->uid)
                ),
                render($el),
                bf_dinner_utility::getUserFieldValue($us->uid, DINNER_FIELD_USER_LOAD_TOTAL),
                bf_dinner_utility::getUserFieldValue($us->uid, DINNER_FIELD_USER_LOAD_PRDAY) == 1 ?
                    'Sammen' : 'Alene',
                $us->getAccumulatedPoints(),


            )
        );


    }

    return array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array('id' => 'admin-dblog'),
        '#empty' => t('No log messages available.'),
    );
}

/**
 * @param $uid
 */
function bf_dinner_admin_user ($uid) {
    $us = \Drupal\bf_dinner\Entity\User::load($uid);

    $header = array(
        'Dato',
        'Runde',
        'Status',
        'Status',
        'Enkelt / dobbelt',
        '',
        'Menu',

    );

    $rows = [];
    /** @var \Drupal\bf_dinner\Entity\Date $date */
    foreach ($us->getAssignedDates() as $date) {
        $classes = [];

        /** @var \Drupal\bf_dinner\Entity\User $u */
        foreach ($date->getUsers() as $u) {
            $userNames[] = $u->getName();
        }

        $rows[] = array(
            'data' => array(
                sprintf('<a href="/node/%d/edit#kokke">%s</a>', $date->id(), $date->getDateString()),
                $date->getSeriesString(),
                $date->status ? 'Udgivet' : 'Ikke udgivet',
                $date->getStateString(),
                $date->getLoadString(),
//                implode(',', $userNames),
                implode(',', $date->getTempAssignmentUids()),
                $date->getFieldValue('body')

            ),
            'class' => array(
                $date->getStateCssClass(),
                $date->status ? 'published' : 'node-unpublished',
                $date->canBePublished() ? 'can-be-published' : 'can-not-be-published',
            )
        );


    }

    $table = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array('id' => ''),
        '#empty' => t('No log messages available.'),
    );


    return array(
        $table,
    );
}

/**
 *
 */
function bf_dinner_admin_dates() {
    $header = array(
        '',
        'Runde',
        'Dato',
        'Status',
        'Status',
        'Enkelt / dobbelt',
        '',
        'Menu',

    );

    $rows = [];
    foreach (bf_dinner_utility::getAllFutureNodes() as $node) {

        $classes = [];


        $rows[] = array(
            'data' => array(
                sprintf('<input type="checkbox" name="date[]" value="%s"/>', $node->getDateString()),
                $node->getSeriesString(),
                sprintf('<a href="/node/%d/edit?destination=admin/dinner/dates#kokke">%s</a>', $node->id(), $node->getDateString()),
                $node->status ? 'Udgivet' : 'Ikke udgivet',
                $node->getStateString(),
                $node->getLoadString(),
                implode(',', $node->getTempAssignmentUids()),
                $node->getFieldValue('body')

            ),
            'class' => array(
                $node->getStateCssClass(),
                $node->status ? 'published' : 'node-unpublished',
                $node->canBePublished() ? 'can-be-published' : 'can-not-be-published',
            )
        );


    }

    return array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array('id' => 'admin-dblog'),
        '#empty' => t('No log messages available.'),
    );

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

	$form['bf_dinner_days_from_deadline_to_first'] = array(
		'#type' => 'date_popup',
		'#title' => 'Dage fra deadline til første dag',
		'#date_format' => 'd-m-Y',
		'#default_value' => variable_get('bf_dinner_next_deadline'),
	);

	$form['bf_dinner_days_from_open_to_deadline'] = array(
		'#type' => 'textfield',
		'#title' => 'Dage fra udsendelse til deadline',
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

function bf_dinner_admin_info_create() {
    return array(
        '#theme' => 'item_list',
        '#items' => array(
            'Antal ikke-publicerede fællesspisninger : ' . sizeof(bf_dinner_utility::getAllUnpublishedNodes()),
            'Sidste publicerede fællesspisning : ' . bf_dinner_utility::getLastPublishedDate()->format('d.m.Y'),
            'Antal kokke : ' . bf_dinner_utility::getUserCount(),
            'Næste kan-liste anbefales derfor : ' . bf_dinner_utility::getNextSeriesFirstDateSuggestion()->format('d.m.Y') . ' - ' . bf_dinner_utility::getNextSeriesLastDateSuggestion()->format('d.m.Y') . ' (begge inkl.)',
        ),
        '#prefix' => '<div class="stats-info">',
        '#suffix' => '</div>',
    );
}


function bf_dinner_admin_info_publish () {
    return array(
        '#theme' => 'item_list',
        '#items' => array(
            'Sidste publicerede fællesspisning : ' . bf_dinner_utility::getLastPublishedDate()->format('d.m.Y'),
            'Antal ikke-publicerede fællesspisninger : ' . sizeof(bf_dinner_utility::getAllUnpublishedNodes()),
            '- heraf med kokke (offentliggøres) : ' . sizeof(bf_dinner_utility::getAllUnpublishedNodesWUsers()),
            '- heraf uden kokke (overføres til næste runde) : ' . sizeof(bf_dinner_utility::getAllUnpublishedNodesWUsers(TRUE)),
        ),
        '#prefix' => '<div class="stats-info">',
        '#suffix' => '</div>',
    );
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

/**
 * @param $date
 * @param $assigned
 */
function bf_dinner_assignment_ajax ($uid, $date, $assigned) {

	$date = new DateTime($date);
	$success = bf_dinner_utility::setDateUserAssignment($uid, $date, $assigned);

	$out = array(
		'assigned' => $assigned ? 1 : 0,
		'errorCode' => $success ? 0 : 1,
		'errorMessage' => $success ? '' : 'Some error',
	);
	drupal_json_output($out);
}