<?php

/*
 * Dernières modifications: 02/06/2021 Romain Neil
 */

function minicals($year, $month, $day, $id_classe, $type, $dmy) {

	// PHP Calendar Class
	//
	// Copyright David Wilkinson 2000. All Rights reserved.
	//
	// This software may be used, modified and distributed freely
	// providing this copyright notice remains intact at the head
	// of the file.
	//
	// This software is freeware. The author accepts no liability for
	// any loss or damages whatsoever incurred directly or indirectly
	// from the use of this script.
	//
	// URL:   http://www.cascade.org.uk/software/php/calendar/
	// Email: davidw@cascade.org.uk

	class Calendar {
		var $month;
		var $year;
		var $day;
		var $h;
		var $id_classe;
		var $type;
		var $dmy;

		function __construct($day, $month, $year, $h, $id_classe, $type, $dmy) {
			$this->day = $day;
			$this->month = $month;
			$this->year = $year;
			$this->h = $h;
			$this->id_classe = $id_classe;
			$this->type = $type;
			$this->dmy = $dmy;
		}

		function getCalendarLink($month, $year, $id_classe, $type) {
			$day = 1;
			if ($this->dmy == 'gestion_absences')return $this->dmy . ".php?year=$year&month=$month&day=$day&id_classe=$id_classe&type=$type";
		}

		function getDateLink($day, $month, $year) {
			global $vocab;
			if ($this->dmy == 'gestion_absences') return "<a title=\"Voir le cahier de textes pour cette journée\" href=" . $this->dmy . ".php?year=$year&month=$month&day=$day&id_classe=" . $this->id_classe . "&type=" . $this->type;
		}

		function getDaysInMonth($month, $year) {
			if ($month < 1 || $month > 12) {
				return 0;
			}

			$days = array(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);

			$d = $days[$month - 1];

			if ($month == 2) {
				// Check for leap year
				// Forget the 4000 rule, I doubt I'll be around then...

				if ($year % 4 == 0) {
					if ($year % 100 == 0) {
						if ($year % 400 == 0) {
							$d = 29;
						}
					} else {
						$d = 29;
					}
				}
			}

			return $d;
		}

		function getFirstDays() {
			global $weekstarts;

			$basetime = mktime(12, 0, 0, 6, 11 + $weekstarts, 2000);
			for ($i = 0, $s = ""; $i < 7; $i++) {
				$show = $basetime + ($i * 24 * 60 * 60);
				$fl = strftime('%a', $show);
				$s .= "<td align=center valign=top class=\"calendarHeader\">$fl</td>\n";
			}
			return $s;
		}

		function getHTML() {
			global $weekstarts;
			global $vocab;
			if (!isset($weekstarts)) $weekstarts = 0;
			$s = "";

			$daysInMonth = $this->getDaysInMonth($this->month, $this->year);
			$date = mktime(12, 0, 0, $this->month, 1, $this->year);

			$first = (strftime("%w", $date) + 7 - $weekstarts) % 7;
			$monthName = strftime("%B", $date);

			$prevMonth = $this->getCalendarLink($this->month - 1 > 0 ? $this->month - 1 : 12, $this->month - 1 > 0 ? $this->year : $this->year - 1, $this->id_classe, $this->type);
			$nextMonth = $this->getCalendarLink($this->month + 1 <= 12 ? $this->month + 1 : 1, $this->month + 1 <= 12 ? $this->year : $this->year + 1, $this->id_classe, $this->type);

			$s .= "<table class=\"calendar\" style=\"margin:auto\" border=\"1\" summary=\"Tableau calendrier\">\n";
			$s .= "<tr><td class=\"calendarcol1\" align='right' valign='top'></td>\n";
			$s .= "<td align='center' valign='top' class=\"calendarHeader\">" . (($prevMonth == "") ? "&nbsp;" : "<a href=\"$prevMonth\">&lt;&lt;</a>") . "</td>\n";
			$s .= "<td align='center' valign='top' class=\"calendarHeader\" colspan='5'>";

			if ($this->dmy != 'gestion_absences') {
				$s .= "<a href=\"month.php?year=$this->year&month=$this->month&day=1&id_classe=$this->id_classe&type=$this->type\">$monthName&nbsp;$this->year</a>";
			} else {
				$s .= "$monthName&nbsp;$this->year";
			}

			$s .= "</td>\n";
			$s .= "<td align=center valign=top class=\"calendarHeader\">" . (($nextMonth == "") ? "&nbsp;" : "<a href=\"$nextMonth\">&gt;&gt;</a>") . "</td>\n";
			$s .= "</tr>\n";

			$s .= "<tr><td  class=\"calendarcol1\" align=right valign=top></td>\n";
			$s .= $this->getFirstDays();
			$s .= "</tr>\n";

			$d = 1 - $first;
			$temp = 1;
			$numweek = 1;
			while ($d <= $daysInMonth) {
				$s .= "<tr><td class=\"calendarcol1\" align=right valign=top>";
				if ($this->dmy != 'gestion_absences') {
					$s .= "<a href=\"week.php?year=$this->year&month=$this->month&day=$temp&id_classe=$this->id_classe&type=$this->type\">s" . $numweek . "</a>";
					$temp = $temp + 7;
					$numweek++;
				}
				$s .= "</td>\n";
				for ($i = 0; $i < 7; $i++) {
					$s .= "<td class=\"calendar\" align=right valign=top>";
					if ($d > 0 && $d <= $daysInMonth) {
						$link = $this->getDateLink($d, $this->month, $this->year);
						if ($link == "")
							$s .= $d;
						elseif (($d == $this->day) and ($this->h))
							$s .= $link . "><span class=\"cal_current_day\">$d</span></a>";
						else
							$s .= $link . ">$d</a>";
					} else {
						$s .= "&nbsp;";
					}
					$s .= "</td>\n";
					$d++;
				}
				$s .= "</tr>\n";
			}

			$s .= "</table>\n";

			return $s;
		}
	}

	$thismonth = mktime(0, 0, 0, $month, $day, $year);


	$cal = new Calendar(date("d", $thismonth), date("m", $thismonth), date("Y", $thismonth), 1, $id_classe, $type, $dmy);
	echo $cal->getHTML();

}
