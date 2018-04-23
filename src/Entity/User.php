<?php
/**
 * Created by PhpStorm.
 * User: larskoie
 * Date: 28/12/2016
 * Time: 22.32
 */

namespace Drupal\bf_dinner\Entity;


class User
{

    public $uid;

    public function __construct() {

    }

    /**
     * @param $uid
     * @return User
     */
    public static function load($uid) {
        return \bf_dinner_utility::makeUserObjectFromStdClass($uid);
    }

    public function getFieldValue ($field) {
        if ($items = field_get_items('user', $this, $field)) {
            return $items[0]['value'];
        }
        return '';
    }

    public function getName () {
        return $this->getFieldValue(DINNER_FIELD_USER_NAME);
    }

    /**
     * Permanently assigned
     *
     */
    public function getAssignedDates () {
        $res = \bf_dinner_utility::getEntityFieldQuery()
            ->fieldCondition(DINNER_FIELD_USERS, 'target_id', $this->uid)
            ->fieldOrderBy(DINNER_FIELD_DATE_DATE, 'value', 'desc')
            ->execute();

        $dates = [];
        foreach ($res['node'] as $node) {
            $date = \bf_dinner_utility::makeDateObjectFromStdClass($node);
            $dates[] = $date;
        }

        return $dates;
    }

    /**
     *
     */
    public function getAccumulatedPoints () {
        foreach ($this->getAssignedDates() as $date) {
        }
    }

}