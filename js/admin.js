/**
 * Created by larskoie on 18/08/16.
 */


jQuery(document).ready(function () {
    jQuery('.interactive td.status').click(function () {
        var td = jQuery(this);
        var div = td.find('div');
        div.text('.');

        var assigned = td.hasClass('assigned-1');
        var newAssigned = !assigned;
        jQuery.ajax({
            url: '/admin/dinner_assignment_ajax/' + td.data('uid') + '/' + td.data('date') + '/' + (newAssigned ? '1' : '0'),
            success: function (data) {
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

function updateOkClasses() {
    jQuery('th.user').each(function () {
        var uid = jQuery(this).data('uid');
        var userPoints = 0;
        jQuery('.user-' + uid + '.assigned-1').each(function () {
            userPoints += parseInt(config.users[uid].pointsprdate);
        });
//		console.log('userpoints:' + userPoints + ', nominal:' + config.users[uid].pointstotal);
        if (userPoints == config.users[uid].pointstotal) {
            jQuery('.user-' + uid).addClass('user-ok');
        } else {
            jQuery('.user-' + uid).removeClass('user-ok');
        }
    });

    jQuery('th.date').each(function () {
        var date = jQuery(this).data('date');
        var datePoints = 0;
        jQuery('.date-' + date + '.assigned-1').each(function () {
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


var dinner = {};


dinner.el = jQuery('table#assigned-table');

/**
 * 
 * @return {*}
 */
getElement = function () {
    return dinner.el;
};

/**
 *
 * @return {Array}
 */
getDays = function () {
    return dinner.days;
};

/**
 *
 * @return {Array}
 */
getUserGroups = function() {
    return dinner.userGroups;
};

/**
 *
 * @return {Array}
 */
getUsers = function () {
    return dinner.users;
};

/**
 *
 * @param dayData
 * @param userData
 * @param cellData
 * @constructor
 */
Init = function (dayData, userData, cellData) {

    dinner.cellData = cellData;

    dinner.users = [];
    dinner.userGroups = [];
    var count = 0, _groupId = -1;
    jQuery.each(userData, function (i, el) {
        user = new User(i, count);
        var groupId = Math.floor(count / 2);
        var userGroup = getUserGroup(groupId);
        user.setFirstInGroup(groupId !== _groupId);
        if (! userGroup) {
            userGroup = new UserGroup(groupId);
            dinner.userGroups.push(userGroup);
        }
        userGroup.addUser(user);
        user.setGroup(userGroup);
        user.setNominalPoints(el.pointstotal);
        dinner.users.push(user);

        count ++;
        _groupId = groupId;
    });
    dinner.days = [];
    dinner.cells = [];
    count = 0;
    jQuery.each(dayData, function (i, el) {
        day = new Day(i, count++);
        day.setNominalPoints(el.pointstotal);
        dinner.days.push(day);

    });

    console.log(dinner.days);
    console.log(dinner.users);
    console.log(dinner.cells);
    console.log(dinner.userGroups);

};

/**
 *
 * @return {Array}
 */
getCells = function() {
    return dinner.cells;
};

/**
 *
 * @return {Array}
 */
getAssignedDays = function () {
    var days = [];
    jQuery.each(getDays(), function(i, day){
        if (day.getPoints() > 0) {
            days.push(day);
        }
    });
    return days;
};

/**
 *
 * @return {Array}
 */
getAssignedCells = function () {
    var assignedCells = [];
    jQuery.each(getCells(), function(i, cell){
        if (cell.getPoints() > 0) {
            assignedCells.push(cell);
        }
    });
    return assignedCells;
};
getUnassignedCells = function () {
    var unassignedCells = [];
    jQuery.each(getCells(), function(i, cell){
        if (cell.getPoints() == 0) {
            unassignedCells.push(cell);
        }
    });
    return unassignedCells;
};

getUserGroup = function (gid) {
  for (var t=0; t<dinner.userGroups.length; t++) {
      if (dinner.userGroups[t].getGid() == gid) {
          return dinner.userGroups[t];
      }
  }
  return false;
};

getCell = function (user, day) {
    jQUery.each(dinner.cells, function (i, cell) {
        if (cell.getDateString() == day.getDateString()
            &&
            cell.getUser().getUid() == user.getUid()) {
            return cell;
        }
    });
};

OutputMsg = function (msg) {
    jQuery('#messages').prepend(
        jQuery('<div/>').text(msg)
    );

};

/**
 *
 * @param dateString
 * @returns {{getDateString: getDateString, setPoints: setPoints, getPoints: getPoints, setNominalPoints: setNominalPoints, getNominalPoints: getNominalPoints}}
 * @constructor
 */
class Day {

    constructor(dateString, index) {
        this.dateString = dateString;
        this.index = index;
        this.points = 0;
        this.nominalPoints = 2;

        this.el = jQuery('<tr/>');
        this.header = jQuery('<th/>');

        this.el.append(this.header);
        dinner.el.append(this.el);

        this.cells = [];

        var day = this;
        jQuery.each(dinner.users, function (i, user) {
            var cell = new Cell(day, user);

            dinner.cells.push(cell);
            day.addCell(cell);
            user.addCell(cell);

        });

        this.updateTable();
    }

    getDateString() {
        return this.dateString;
    }
    getIndex() {
        return this.index;
    }


    addCell(cell) {
        this.cells.push(cell);
    }

    setPoints(points) {
        this.points = points;
    }

    getPrevious() {
        if (this.index == 0) {
            return false;
        }
        return dinner.days[this.index - 1];
    }
    getNext() {
        if (this.index + 1 == dinner.days.length ) {
            return false;
        }
        return dinner.days[this.index + 1];
    }

    updatePoints() {
        var points = 0;
        jQuery.each(this.getAssignedCells(), function (i, cell) {
            points += 2;
        });
        this.points = points;
        this.updateOk();
        this.updateTable();
    }

    updateOk() {
        this.ok = this.points == this.nominalPoints;
        OutputMsg(this.ok);
    }

    updateTable() {
        var day = this;
        this.header.html(this.getDateString() + '<br/>' + this.getPoints() + ':' + this.getNominalPoints() + ':' + (this.ok ? 1 : 0));
        jQuery.each(day.getCells(), function (i, cell) {
            if (day.ok) {
                cell.el.addClass('date-ok');
            } else {
                cell.el.removeClass('date-ok');
            }
        });
    }

    getPoints() {
        return this.points;
    }


    setNominalPoints(points) {
        this.nominalPoints = points;
    }


    getNominalPoints() {
        return this.nominalPoints;


    }

    getOk() {
        return this.ok;
    }

    getCells() {
        return this.cells;
    }

    getCellByUser (user) {
        var out = null;
        jQuery.each(this.getCells(), function(i, cell){
            if (cell.getUser().getUid() == user.getUid()) {
                out = cell;
            }
        });
        if (out) {
            return out;
        }
        return false;
    }

    /**
     *
     * @return {Array}
     */
    getPossibleCells() {
        var cells = [];
        jQuery.each(this.cells, function (i, cell) {
            if (cell.getPossible()) {
                cells.push(cell);
            }
        });
        return cells;
    }

    /**
     *
     * @return {Array}
     */
    getAssignedCells() {

        var cells = [];
        jQuery.each(this.cells, function (i, cell) {

            if (cell.getAssigned()) {
                cells.push(cell);
            }
        });

        return cells;
    }
}

/**
 *
 */
class UserGroup {

    /**
     *
     * @param gid
     */
    constructor(gid) {
        this.gid = gid;
        this.users = [];
    }

    getGid() {
        return this.gid;
    }

    /**
     *
     * @param user
     */
    addUser(user) {
        this.users.push(user);
    }

    /**
     *
     */
    getUsers() {
        return this.users;
    }

    /**
     *
     */
    updateDistances() {
        for (var i=0; i<this.getUsers().length; i++) {
            this.getUsers()[i].updateDistances();
        }
    }

    /**
     *
     */
    updateTable() {
        for (var i=0; i<this.getUsers().length; i++) {
            this.getUsers()[i].updateTable();
        }
    }

}

class User {

    constructor(uid, index) {
        this.uid = uid;
        this.index = index;

        this.group = Math.floor(index / 2);

        this.points = 0;
        this.nominalPoints = 0;
        this.pointsPrDay = 0;

        this.header = jQuery('<th/>');
        dinner.el.find('tr').append(this.header);

        this.cells = [];
        this.updateTable();
    }

    is(user) {
        return (this.getUid() == user.getUid());
    }

    setGroup (userGroup) {
        this.userGroup = userGroup;
    }
    getGroup () {
        return this.userGroup;
    }

    setFirstInGroup(firstInGroup){
        this.firstInGroup = firstInGroup;
    }
    getFirstInGroup(){
        return this.firstInGroup;
    }
    setLastInGroup(lastInGroup){
        this.lastInGroup = lastInGroup;
    }
    getLastInGroup(){
        return this.lastInGroup;
    }

    addCell(cell) {
        this.cells.push(cell);
    }

    getUid() {
        return this.uid;
    }
    getIndex() {
        return this.index;
    }

    getOk() {
        return this.ok;
    }

    updatePoints() {
        var points = 0;
        jQuery.each(this.getAssignedCells(), function (i, cell) {
            points += 2;
        });
        this.points = points;
        this.updateOk();
        this.updateTable();

    }

    /**
     *
     */
    updateDistances () {

        var assigned = [];
        var usersInGroup = this.getGroup().getUsers();
        for (var i=0; i<usersInGroup.length; i++) {
            for (var j=0; j<usersInGroup[i].getCells().length; j++) {
                if (usersInGroup[i].getCells()[j].getAssigned()) {
                    assigned.push(j);
                }
            }
        }
        console.log(this.getGroup());
        console.info(assigned, 'assigned');
        jQuery.each(this.getCells(), function(i, cell){
            var min = 100;
            for (var t=0; t<assigned.length; t++) {
                min = Math.min(min, Math.abs(i - assigned[t]));
            }
            cell.setDistanceToNearestAssigned(min);
        });
    }

    updateOk() {
        this.ok = this.points == this.nominalPoints;
    }


    /**
     *
     */
    updateTable() {
        var user = this;
        this.header.html(this.uid + '/' + (this.getGroup() ? this.getGroup().getGid() : '') + '<br/>' + this.points);

        jQuery.each(user.getCells(), function (i, cell) {

            cell.updateTable();

            if (user.ok) {
                cell.el.addClass('user-ok');
            } else {
                cell.el.removeClass('user-ok');
            }

        });

    }

    /**
     *
     * @param points
     */
    setPoints(points) {
        this.points = points;
    }


    getPoints() {
        return this.points;
    }


    setNominalPoints(points) {
        this.nominalPoints = points;
    }


    getNominalPoints() {
        return this.nominalPoints;
    }

    setPointsPrDay(points) {
        this.pointsPrDay = points;
    }

    getPointsPrDay() {
        return this.pointsPrDay;
    }

    /**
     * @return array
     */
    getCells() {
        return this.cells;
    }

    getCellByDay (day) {
        jQuery.each(this.getCells(), function(i, cell){
            if (cell.getDay().getDateString() == day.getDateString()) {
                return cell;
            }
        });
        return false;
    }

    /**
     * @return array
     */
    getPossibleCells() {

        var cells = [];
        jQuery.each(this.cells, function (i, cell) {
            if (cell.getPossible()) {
                cells.push(cell);
            }
        });
        return cells;
    }

    /**
     * @return array
     */
    getAssignedCells() {
        var cells = [];
        jQuery.each(this.cells, function (i, cell) {
            if (cell.getAssigned()) {
                cells.push(cell);
            }
        });
        return cells;
    }


}


class Cell {

    constructor(day, user) {

        var cell = this;
        this.day = day;
        this.user = user;

        this.status = -1;
        this.assigned = 0;
        this.distanceToNearestAssigned = 100;

        this.el = jQuery('<td/>');
        this.el.addClass('status');
        if (this.user.getFirstInGroup()) {
            this.el.addClass('first-in-group');
        }
        this.el.click(function () {
            cell.onClick();
        });
        this.setStatus(dinner.cellData[day.dateString][this.user.uid]);
        day.el.append(this.el);

    }

    /**
     *
     * @return {*}
     */
    getDay() {
        return this.day;
    }

    /**
     *
     * @return {*}
     */
    getUser() {
        return this.user;
    }

    /**
     *
     * @return {*}
     */
    getStatus() {
        return this.status;
    }

    /**
     *
     * @param status
     */
    setStatus(status) {
        OutputMsg('Setting status to ' + status);
        this.status = status;
        this.el.removeClass('status-unknown status-possible status-not-possible');
        switch (this.status) {
            case -1 :
                this.el.addClass('status-unknown');
                break;

            case 0 :
                this.el.addClass('status-not-possible');
                break;

            case 1 :
                this.el.addClass('status-possible');
                break;
        }
    }

    /**
     *
     */
    getPossible () {

        if (this.getDistanceToNearestAssigned() <= 3) {
            return false;
        }

        return this.getStatus() == 1
            || this.getStatus() == -1
    }

    /**
     *
     * @param distance
     */
    setDistanceToNearestAssigned(distance){
        this.distanceToNearestAssigned = distance;
    }

    /**
     *
     * @return {number|*}
     */
    getDistanceToNearestAssigned(){
        return this.distanceToNearestAssigned;
    }

    /**
     *
     * @return {number|*}
     */
    getAssigned() {
        return this.assigned;
    }

    /**
     *
     * @param assigned
     */
    setAssigned(assigned) {
        this.assigned = assigned;
        this.updatePoints();
        this.updateTable();
        this.getUser().getGroup().updateDistances();
        this.getUser().getGroup().updateTable();
    }

    /**
     *
     */
    updateTable() {
        if (this.assigned) {
            this.el.text('O');
        } else {
            if (this.distanceToNearestAssigned <= 3) {
                this.el.text('-');
            } else {

                this.el.text('');

            }
        }

    }

    /**
     *
     */
    toggleAssigned() {
        this.setAssigned(! this.assigned);
        this.updatePoints();
    }

    /**
     *
     */
    updateOk() {

    }

    /**
     *
     */
    updatePoints() {

        this.points = this.assigned ? 2 : 0;
        this.day.updatePoints();
        this.user.updatePoints();

    }


    /**
     *
     */
    onClick() {
        OutputMsg('click ' + this.day.getDateString() + ' ' + this.user.uid);


        this.toggleAssigned();

    }

}


class Assigner {

    constructor(dinner) {
        var assigner = this;
        this.days = dinner.days;
        this.cells = dinner.cells;
        jQuery.each(this.cells, function(i, cell){
            cell.el.click(function(){
                assigner.onClick(cell);
            });
        });
    }

    go() {
        for (var t = 0; t < 50; t++) {
            this.iteration();


        }
    }

    onClick(cell){
        OutputMsg('assigner click ' + cell.getDay().getDateString());
    }

    iteration() {
        var assigner = this;
        var changed = false;

        this.fillHoles();

        // no holes found - move to step 2
        if (0 && ! changed) {
            var randomDay = assigner.getRandomAssignedDay();
            jQuery.each(randomDay.getAssignedCells(), function(i, randomCell) {

                var randomUser = randomCell.getUser();
                randomCell.setAssigned(false);
                day.getCellByUser(randomUser).setAssigned(true);
            });



        }

    }

    /**
     *
     * "Fill holes" means assign the green ones until there are no more green ones.
     */
    fillHoles () {
        var assigner = this;
        var changed = false;

        // step 1 - find holes
        jQuery.each(this.days, function (i, day) {
            if (day.getPoints() < day.getNominalPoints()) {
                jQuery.each(day.getPossibleCells(), function (i, cell) {
                    if (!changed) {

                        var user = cell.getUser();

                        if (user.getPoints() < user.getNominalPoints()) {

                            if (!cell.getAssigned()) {
                                cell.setAssigned(true);
                                changed = true;

                            }
                        }
                    }
                });



            }
        });
    }


    /**
     *
     * @param user
     * @param fromDay
     * @param toDay
     */
    moveOneUserFromDayToDay (user, fromDay, toDay){
        jQuery.each(fromDay.getCells(), function(i, fromCell){
            if (fromCell.getAssigned()) {
                var u = fromCell.getUser();
                if (u.is(user)) {
                    var toCell = toDay.getCellByUser(u);
                    toCell.setAssigned(true);
                    fromCell.setAssigned(false);
                    OutputMsg('moving user ' + user.getUid() + ' from ' + fromDay.getDateString() + ' to ' + toDay.getDateString());
                }
            }
        });

    }

    moveUsersFromDayToDay (fromDay, toDay) {
        jQuery.each(fromDay.getCells(), function(i, fromCell){
            if (fromCell.getAssigned()) {
                var user = fromCell.getUser();
                var toCell = toDay.getCellByUser(user);
                toCell.setAssigned(true);
                fromCell.setAssigned(false);
                OutputMsg('moving user ' + user.getUid() + ' from ' + fromDay.getDateString() + ' to ' + toDay.getDateString());
            }
        });
    }

    getRandomAssignedDay() {
        var randomDay;
        var assignedDays = getAssignedDays();
        if (assignedDays.length === 0) {
            return false;
        }
        randomDay = assignedDays[Math.floor(assignedDays.length * Math.random(45))];
        return randomDay;

    }


}


var d;

jQuery(document).ready(
    function () {

        Init(config.dates, config.users, config.statuses);
        var a = new Assigner(dinner);
        a.go();
    }
)
;
