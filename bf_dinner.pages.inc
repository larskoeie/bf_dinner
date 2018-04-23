<?php


/**
 * Form that allows user to change dinner statuses in db.
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function bf_dinner_user_status_form($form, &$form_state)
{
    if (!variable_get('bf_dinner_open', FALSE)) {
        $form['info'] = array(
            '#markup' => 'Der er ingen åben kan-liste for øjeblikket.',
            '#prefix' => '<div>',
            '#suffix' => '</div>',
        );

        return $form;

    }

    setlocale(LC_ALL, 'da_DK.UTF-8');

    // add logged in user to array of users in house
    global $user;
    $uid = $user->uid;
    $user = user_load($uid);


    bf_dinner_utility::setUserFormLastOpenDate($uid);
    $GLOBALS['bf_dinner_is_dinner_page'] = TRUE;

    $users = array($uid => bf_dinner_utility::getUserName($uid));
    $usersWithShortcut = array(0 => 'begge', $uid => bf_dinner_utility::getUserName($uid));

    // get house and add other users in house to array
    $uids = array($uid);


    foreach (bf_dinner_utility::getOtherUsersInHouse($uid) as $u => $dummy) {
        $us = user_load($u);
        $users[$u] = bf_dinner_utility::getUserName($u);
        $usersWithShortcut[$u] = bf_dinner_utility::getUserName($u);
        $uids[$u] = $u;
    }

    $conflictRiscArray = bf_dinner_utility::getConflictRiscArray($uid);
    $hasConflictRisc = !empty($conflictRiscArray['risc']);
    if (!empty($hasConflictRisc)) {
        drupal_set_message($conflictRiscArray['messagePage'], 'warning');
    }

    $allowOtherCheckboxes = !$hasConflictRisc;

    $usersWithShortcut[0] = implode(' og ', $users);

    $form['info'] = array(
        '#markup' => variable_get('bf_dinner_header'),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );
    $deadline = new \DateTime(variable_get('bf_dinner_next_deadline', ''));
    $form['info2'] = array(
        '#markup' => 'Deadline : ' . $deadline->format('d. m. Y'),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );

    // the value of this form field is not stored on server - could be stored in cookie
    // @TODO : disable or hide this if there are no other_users
    // @TODO : OR other_users has opened the form recently
    if ($allowOtherCheckboxes) {
        $form['show_other_users'] = array(
            '#title' => 'Udfyld kan-liste(r)',
            '#type' => 'select',
            '#options' => array(
                0 => 'udfyld kun min egen kan-liste',
                1 => 'udfyld alle husstandens kan-lister samtidig',
            ),
            '#default_value' => 0,
        );
    }

    // the value of this form field is stored in a field on user's account
    foreach ($users as $u => $name) {
        $first = 0;
        $form['points-' . $u] = array(
            '#title' => $name . ' vil gerne lave mad',
            '#type' => 'select',

            '#options' => array(
                DINNER_LOAD_SINGLE => 'alene',
                DINNER_LOAD_HALF => 'sammen med en anden Blikboer',
            ),
            '#default_value' => bf_dinner_utility::getUserLoadPrDay($u),
            '#attributes' => array(
                'class' => array($u == $uid ? 'user-self' : 'user-other'),
            ),
            '#prefix' => '<div class="bf-dinner-pref-user">',

        );


        $form['kids-' . $u] = array(
            '#title' => $name . ' vil gerne lave "Mad med børn"',
            '#type' => 'select',
            '#options' => array(
                0 => 'nej tak',
                1 => 'ja tak',
            ),
            '#default_value' => bf_dinner_utility::getUserFieldValue($u, DINNER_FIELD_USER_KIDS, 0),
            '#suffix' => '</div>',
            '#attributes' => array(
                'class' => array($u == $uid ? 'user-self' : 'user-other'),
            ),
        );


    }

    if (FALSE) {
        $form['halfloadtype'] = array(
            '#title' => 'Jeg/vi vil gerne lave mad sammen med',
            '#type' => 'select',
            '#options' => array(
                0 => 'vælg tilfældigt',
                2 => 'en gang med hinanden, en gang med andre Blikboere',
            ),
            '#default_value' => bf_dinner_utility::getUserFieldValue($user, ''),
            '#attributes' => array(
                'class' => array(''),
            ),
            '#prefix' => '<div class="bf-dinner-pref-user">',
            '#suffix' => '</div>',
        );
    }

    $_m = '';
    $_w = '';
    $scheduledDatesCount = 0;
    $prDateCounter = 0;
    $last_date_string = '';

    //$nodes = bf_dinner_utility::getAllUnpublishedNodes();
    //  @TODO ! 
    $nodes = bf_dinner_utility::getAllNodesBetweenDates(new DateTime('2018-03-27'), new DateTime('2018-05-15'));
    foreach ($nodes as $node) {
        $classes = array();
        $node = node_load($node->nid);
        $date = field_get_items('node', $node, 'field_date')[0]['value'];
        $date = new \DateTime($date);
        $date_string = $date->format('Y-m-d');

        if ($date_string == $last_date_string) {
            $prDateCounter++;
            $date_string .= '-' . $prDateCounter;
        } else {
            $prDateCounter = 0;

        }
        // echo $date_string . "\n";
        $last_date_string = $date->format('Y-m-d');

        $description = '';

        $dateLoad = bf_dinner_utility::getDateLoad($node);
        $body = field_get_items('node', $node, 'body')[0]['value'];
        // not used pt.
        // $dateState = field_get_items('node', $node, DINNER_FIELD_DATE_STATE)[0]['value'];

        // the title next to the checkbox(es)
        $title = strftime('%A d. %e.', $date->getTimestamp());
        if (function_exists('_views_holidays_get_holidays')) {
            $title .= _views_holidays_get_holidays($date);
        }
        if (strlen(trim($body))) {
            $title .= '&nbsp;:&nbsp;' . $body;
        }

        // month label
        $newMonth = strftime('%B', $date->getTimestamp()) != $_m;
        $newWeek = strftime('%V', $date->getTimestamp()) != $_w;
        $_m = strftime('%B', $date->getTimestamp());
        $_w = strftime('%V', $date->getTimestamp());

        $weekLabel = strftime('uge %V', $date->getTimestamp());
        if (strftime('%V', $date->getTimestamp()) == 42) {
            $weekLabel .= ' - skolernes efterårsferie';
        }

        if ($newMonth && $newWeek) {
            $form['month-' . $date->format('m')] = array(
                '#type' => 'markup',
                '#markup' => ucfirst(strftime('%B - uge %V', $date->getTimestamp())),
                '#prefix' => '<h2 class="header-month header-week">',
                '#suffix' => '</h2>',
            );

        } elseif ($newMonth) {

            $form['month-' . $date->format('m')] = array(
                '#type' => 'markup',
                '#markup' => ucfirst(strftime('%B', $date->getTimestamp())),
                '#prefix' => '<h2 class="header-month">',
                '#suffix' => '</h2>',
            );
        } elseif ($newWeek) {
            $form['week-' . $date->format('W')] = array(
                '#type' => 'markup',
                '#markup' => $weekLabel,
                '#prefix' => '<h2 class="header-week">',
                '#suffix' => '</h2>',
            );

            // $title .= $date->format('\U\g\e W') . ')';
        }

        // fetch other event nodes from db
        $q = new EntityFieldQuery();
        $temp = $q
            ->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'event')
            ->fieldCondition('field_date', 'value', $date->format('Y-m-d'))
            ->execute();

        $otherEvents = node_load_multiple(empty($temp['node']) ? array() : array_keys($temp['node']));

        if (sizeof($otherEvents)) {
            $otherEventTitles = array();
            foreach ($otherEvents as $otherEvent) {
                $otherEventTitles[] = $otherEvent->title;
            }
            $description .= implode(', ', $otherEventTitles);
        }

        // weekday
        $isWeekend = $date->format('N') > 5;
        if ($isWeekend) {
            $classes[] = 'weekend';
        }
        if ($dateLoad > 0) {
            if ($dateLoad == DINNER_LOAD_DOUBLE) {
                $title .= ' (2 kokke)';
            }

            // scheduled
            $scheduledDatesCount++;

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
                '#description' => $description,
                '#default_value' => $default,
                '#attributes' => array(
                    'class' => $classes,
                )
            );
        } else {
            // not scheduled
            $form['date-' . $date_string . '-status'] = array(
                '#type' => 'checkboxes',
                '#title' => $title,
                '#description' => $description,
                '#options' => array(
                    0 => $body
                ),
                '#attributes' => array(
                    'class' => array('dinner-state-not-scheduled'),
                )
            );

        }
    }

    $form['summary'] = array(
        '#type' => 'markup',
        '#markup' => sprintf('Ialt %d fællesspisninger', $scheduledDatesCount),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );

    $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Gem kan-liste',
    );

    $form['#submit'][] = 'bf_dinner_user_status_form_submit';
    $form['#attached']['js'] = array(
        drupal_get_path('module', 'bf_dinner') . '/js/form.js',
    );
    $form['#attached']['css'] = array(
        drupal_get_path('module', 'bf_dinner') . '/css/bf_dinner.css',
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
function bf_dinner_user_status_form_submit($form, &$form_state)
{
    global $user;
    $uid = $user->uid;

    $values = $form_state['values'];
    $show_other_users = (bool)$values['show_other_users'];

    foreach ($values as $field => $value) {
        // @TODO : check if this is user-self OR show-user-other is active. To avoid edit conflict.
        if (preg_match('/points-([0-9]+)/', $field, $m)) {
            $u = $m[1];
            if ($u == $uid || $show_other_users) {
                bf_dinner_utility::setUserFieldValue($u, DINNER_FIELD_USER_LOAD_PRDAY, $value);
            }
        }
        if (preg_match('/kids-([0-9]+)/', $field, $m)) {
            $u = $m[1];
            if ($u == $uid || $show_other_users) {
                bf_dinner_utility::setUserFieldValue($u, DINNER_FIELD_USER_KIDS, $value);
            }
        }

        if (preg_match('/date-([0-9]+-[0-9]+-[0-9]+(-[0-9]+)?)-status/', $field, $m)) {
            foreach ($value as $u => $val) {
                if ($u) {
                    if ($u == $uid || $show_other_users) {
                        bf_dinner_utility::setUserDateStatus($u, $m[1], $val ? 1 : 0);
                    }
                }

            }
        }
    }

    bf_dinner_utility::setUserFormLastOpenDate($uid, FALSE);
    drupal_set_message('Dine kan-liste krydser er gemt. Du kan åbne kanlisten igen og lave ændringer indtil deadline.');
    drupal_goto('/');
}

function bf_dinner_conflict_ajax()
{
    global $user;
    bf_dinner_utility::setUserFormLastOpenDate($user->uid);
    drupal_json_output(bf_dinner_utility::getConflictRiscArray($user->uid));
}

