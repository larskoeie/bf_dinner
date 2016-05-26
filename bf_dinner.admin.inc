<?php

/**
 *
 */
function bf_dinner_admin_front() {
	return array(
		'create' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '1. Opret',
				'#path' =>  'admin/dinner/create',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3>',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'#type' => 'markup',
				'#markup' => 'Her oprettes fællesspisninger for den kommende periode. Det er disse, der senere bliver til checkbokse på kan-listen'
			)
		),
		'edit' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '2. Rediger',
				'#path' =>  'admin/dinner/distribute',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3>',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'intro' => array(

					'#type' => 'markup',
					'#markup' => 'Efter fællesspisningerne er oprettet, kan de redigeres til at passe til den specifikke periode'
				),
				'list' => array(
					'#theme' => 'item_list',
					'#items' => array(
						'aflyse fællesspisning visse dage, f.eks. hvis fælleshuset er optaget',
						'ændre antallet af kokke fra en til to, f.eks. i forbindelse med arbejdsweekend',
					),
					'#type' => 'ul',
					'#attributes' => array('class' => 'my-list'),
				)
			)
		),
		'distribute' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '3. Fordel',
				'#path' =>  'admin/dinner/distribute',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3>',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'#type' => 'markup',
				'#markup' => ''
			)
		),
		'publish' => array(
			'header' => array(
				'#theme' => 'link',
				'#text' => '4. Offentliggør',
				'#path' =>  'admin/dinner/publish',
				'#options' => array('attributes' => array('title' => $row->title)),
				'#prefix' => '<h3>',
				'#suffix' => '</h3>'
			),
			'content' => array(
				'#type' => 'markup',
				'#markup' => 'Bla bla bla'
			)
		),

	);
}

/**
 *
 */
function bf_dinner_admin_create() {
	$form['email'] = array(
		'#type' => 'markup',
		'#title' => 'Email',
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
function bf_dinner_admin_distribute() {

}

/**
 *
 */
function bf_dinner_admin_publish() {

}
