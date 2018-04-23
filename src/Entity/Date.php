<?php
namespace Drupal\bf_dinner\Entity;

/**
 * Class Date
 * @package Drupal\bf_dinner\Entity
 */
class Date extends \stdClass  // Drupal 8 : extend \Drupal\node\Entity\Node
{

    public $nid;

    public function __construct () {

    }

    public function id() {
        return $this->nid;
    }

    public function getFieldValue ($field) {
        if ($items = field_get_items('node', $this, $field)) {
            return $items[0]['value'];
        }

        return '';
    }

    /**
     * @return bool|\DateTime
     */
    public function getDate () {
        if ($items = field_get_items('node', $this, 'field_date')) {
            return new \DateTime($items[0]['value']);
        }

        return FALSE;
    }

    public function getSeriesString () {
        return $this->getFieldValue(DINNER_FIELD_DATE_SERIES);
    }

    public function getDateString () {
        if ($date = $this->getDate()) {
            return $date->format('Y-m-d');
        }
        return '';
    }

    public function getState ($default = DINNER_STATE_SCHEDULED) {
        if ($temp = field_get_items('node', $this, DINNER_FIELD_DATE_STATE)) {
            return $temp['0']['value'];
        }
        return $default;
    }

    public function getStateString () {

        switch ($this->getState()) {
            case DINNER_STATE_SCHEDULED :
                return 'Afholdes/afholdt';
            case DINNER_STATE_NOT_SCHEDULED :
                return 'Reservation af fælleshus';
        }

        return '';
    }

    public function getStateCssClass () {

        switch ($this->getState()) {
            case DINNER_STATE_SCHEDULED :
                return 'state-scheduled';
            case DINNER_STATE_NOT_SCHEDULED :
                return 'state-not-scheduled';
        }

        return '';
    }

    public function getLoad ($default = DINNER_LOAD_SINGLE) {
        if ($temp = field_get_items('node', $this, DINNER_FIELD_DATE_LOAD)) {
            return $temp['0']['value'];
        }
        return $default;
    }

    public function getLoadString() {
        switch ($this->getLoad()) {
            case DINNER_LOAD_SINGLE :
                return 'Enkelt';
            case DINNER_LOAD_DOUBLE :
                return 'Dobbelt';
        }

        return '';

    }

    public function getLoadPrUser () {
        return round($this->getLoad() / sizeof($this->getUsers()));
    }

    /**
     * From fields
     */
    public function getUsers () {
        $users = [];
        if ($temp = field_get_items('node', $this, DINNER_FIELD_USERS)) {
            $users[] = User::load($temp['0']['target_id']);
        }
        return $users;
    }

    /**
     * Don't change this date format.
     *
     * @return string
     */
    protected function getDateStringForDB () {
        if ($date = $this->getDate()) {
            return $date->format('Y-m-d');
        }
        return '';
    }


    /**
     * Methods for handling temporary assignments.
     *
     *
     */

    public function canBePublished () {
        // already published ?
        if ($this->status) {
            return FALSE;
        }

        if ($this->getState() === DINNER_STATE_SCHEDULED && !sizeof($this->getTempAssignmentUids())) {
            return FALSE;
        }

        return TRUE;
    }

    /**
     * @return array
     */
    public function getTempAssignmentUids() {

        $res = db_select('bf_dinner_user_date_status')
            ->fields('bf_dinner_user_date_status')
            ->condition('date', $this->getDateStringForDB())
            ->condition('assigned', 1)
            ->execute();

        $out = array();
        foreach ($res as $us) {
            $out[] = intval($us->uid);
        }

        return $out;
    }

}