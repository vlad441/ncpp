// Complete documented Date API; fixed UTC fixtures, no clock or timezone changes.
// Invalid/malformed input has no documented error contract and is not probed.
static Date::DValue dateValue(long long year, int month, int day,
                             int hour=0, int minute=0, int second=0){
	Date::DValue value;
	value.year=year; value.month=(char)month; value.day=(char)day;
	value.hour=(char)hour; value.minute=(char)minute; value.seconds=(float)second;
	return value;
}

void Date_documented_test(){
	NextTest("Date default constructor and now bracketed by clock reads");
	long long before=GetTimestamp('s'); Date current; long long sampled=Date::now();
	long long after=GetTimestamp('s');
	TEST_EQ(current.timestamp>=before && current.timestamp<=after, true);
	TEST_EQ(sampled>=before && sampled<=after, true);

	NextTest("Date seconds constructor: zero, positive and negative");
	TEST_EQ(Date(0LL).timestamp, 0LL); TEST_EQ(Date(1LL).timestamp, 1LL);
	TEST_EQ(Date(-1LL).timestamp, -1LL); TEST_EQ(Date(-86400LL, 's').timestamp, -86400LL);
	TEST_EQ(Date(2147483648LL).timestamp, 2147483648LL);

	NextTest("Date milliseconds constructor: exact seconds and subsecond truncation");
	TEST_EQ(Date(0LL, 'm').timestamp, 0LL); TEST_EQ(Date(2000LL, 'm').timestamp, 2LL);
	TEST_EQ(Date(-2000LL, 'm').timestamp, -2LL);
	TEST_EQ(Date(2999LL, 'm').timestamp, 2LL); TEST_EQ(Date(-2999LL, 'm').timestamp, -2LL);

	NextTest("Date string constructor: direct, reverse and ISO");
	TEST_EQ(Date("01.01.1970 00:00:00").timestamp, 0LL);
	TEST_EQ(Date("1970-01-01 00:00:01").timestamp, 1LL);
	TEST_EQ(Date("1969.12.31 23:59:59").timestamp, -1LL);
	TEST_EQ(Date("2000-01-01T00:00:00Z").timestamp, 946684800LL);
	TEST_EQ(Date("02.02.2026 01:09:05").timestamp, 1769994545LL);

	NextTest("Date IMF constructor: published HTTP fixture and epoch");
	TEST_EQ(Date("Sun, 06 Nov 1994 08:49:37 GMT").timestamp, 784111777LL);
	TEST_EQ(Date("Thu, 01 Jan 1970 00:00:00 GMT").timestamp, 0LL);
	TEST_EQ(Date("Wed, 31 Dec 1969 23:59:59 GMT").timestamp, -1LL);

	NextTest("Date DValue constructor: all components and signed epoch");
	TEST_EQ(Date(dateValue(1970,1,1)).timestamp, 0LL);
	TEST_EQ(Date(dateValue(1969,12,31,23,59,59)).timestamp, -1LL);
	TEST_EQ(Date(dateValue(1994,11,6,8,49,37)).timestamp, 784111777LL);
	TEST_EQ(Date(dateValue(2026,2,2,1,9,5)).timestamp, 1769994545LL);
	Date wideYear(dateValue(3000000000LL,3,1,12,34,56));
	TEST_EQ(wideYear.getYear(), 3000000000LL);
	TEST_EQ(wideYear.getMonth(), 3); TEST_EQ(wideYear.getDay(), 1);
	TEST_EQ(wideYear.toTime(), "12:34:56");

	NextTest("Date year, month, day and date getters at epoch boundaries");
	Date epoch(0LL), previous(-1LL), future(2147483648LL);
	TEST_EQ(epoch.getYear(), 1970LL); TEST_EQ(epoch.getMonth(), 1); TEST_EQ(epoch.getDay(), 1);
	TEST_EQ(previous.getYear(), 1969LL); TEST_EQ(previous.getMonth(), 12); TEST_EQ(previous.getDate(), 31);
	TEST_EQ(previous.getDate(), previous.getDay());
	TEST_EQ(future.getYear(), 2038LL); TEST_EQ(future.getMonth(), 1); TEST_EQ(future.getDay(), 19);

	NextTest("Date weekday is Monday zero through Sunday six");
	for(int day=0; day<7; ++day){
		TEST_EQ(Date(345600LL+day*86400LL).getDayWeek(), day); // Monday 1970-01-05
		TEST_EQ(Date(-259200LL+day*86400LL).getDayWeek(), day); // Monday 1969-12-29
	}
	TEST_EQ(Date(-1LL).getDayWeek(), 2); TEST_EQ(Date(-86401LL).getDayWeek(), 1);

	NextTest("Date direct formatting: padding, separator and short time");
	Date fixture(784111777LL);
	TEST_EQ(fixture.toDirectDate(), "06.11.1994 08:49:37");
	TEST_EQ(fixture.toDirectDate('/'), "06/11/1994 08:49:37");
	TEST_EQ(fixture.toDirectDate('-', true), "06-11-1994 08:49");
	TEST_EQ(epoch.toDirectDate(), "01.01.1970 00:00:00");

	NextTest("Date reverse formatting: padding, separator and short time");
	TEST_EQ(fixture.toReverseDate(), "1994.11.06 08:49:37");
	TEST_EQ(fixture.toReverseDate('/'), "1994/11/06 08:49:37");
	TEST_EQ(fixture.toReverseDate('-', true), "1994-11-06 08:49");
	TEST_EQ(epoch.toReverseDate(), "1970.01.01 00:00:00");

	NextTest("Date toString default and character separator");
	TEST_EQ(fixture.toString(), "1994.11.06 08:49:37");
	TEST_EQ(fixture.toString('/'), "1994/11/06 08:49:37");

	NextTest("Date toString documented modes preserve date separator");
	TEST_EQ(fixture.toString("reverse"), "1994.11.06 08:49:37");
	TEST_EQ(fixture.toString("direct"), "06.11.1994 08:49:37");
	TEST_EQ(fixture.toString("reverse", '/'), "1994/11/06 08:49:37");
	TEST_EQ(fixture.toString("direct", '/'), "06/11/1994 08:49:37");
	TEST_EQ(fixture.toString("iso"), "1994-11-06T08:49:37Z");
	TEST_EQ(fixture.toString("imf"), "Sun, 06 Nov 1994 08:49:37 GMT");

	NextTest("Date ISO formatting: zero, negative and beyond 32-bit seconds");
	TEST_EQ(epoch.toISOString(), "1970-01-01T00:00:00Z");
	TEST_EQ(previous.toISOString(), "1969-12-31T23:59:59Z");
	TEST_EQ(future.toISOString(), "2038-01-19T03:14:08Z");

	NextTest("Date IMF formatting: all seven weekday names");
	const char* imfDays[]={"Mon, 05 Jan 1970 00:00:00 GMT", "Tue, 06 Jan 1970 00:00:00 GMT",
		"Wed, 07 Jan 1970 00:00:00 GMT", "Thu, 08 Jan 1970 00:00:00 GMT",
		"Fri, 09 Jan 1970 00:00:00 GMT", "Sat, 10 Jan 1970 00:00:00 GMT", "Sun, 11 Jan 1970 00:00:00 GMT"};
	for(int day=0; day<7; ++day) TEST_EQ(Date(345600LL+day*86400LL).toIMFDate(), imfDays[day]);
	TEST_EQ(fixture.toIMFDate(), "Sun, 06 Nov 1994 08:49:37 GMT");
	TEST_EQ(previous.toIMFDate(), "Wed, 31 Dec 1969 23:59:59 GMT");

	NextTest("Date time formatting: padding and short mode");
	TEST_EQ(epoch.toTime(), "00:00:00"); TEST_EQ(epoch.toTime(true), "00:00");
	TEST_EQ(Date(3723LL).toTime(), "01:02:03"); TEST_EQ(Date(3723LL).toTime(true), "01:02");
	TEST_EQ(Date(86399LL).toTime(), "23:59:59"); TEST_EQ(Date(86400LL).toTime(), "00:00:00");

	NextTest("Date negative seconds use the preceding civil day");
	TEST_EQ(previous.toTime(), "23:59:59"); TEST_EQ(previous.toTime(true), "23:59");
	TEST_EQ(previous.toDirectDate(), "31.12.1969 23:59:59");
	TEST_EQ(previous.toReverseDate(), "1969.12.31 23:59:59");
	TEST_EQ(Date(-86400LL).toTime(), "00:00:00");
	TEST_EQ(Date(-86401LL).toReverseDate(), "1969.12.30 23:59:59");

	NextTest("Date leap-day and non-leap century transitions");
	TEST_EQ(Date(951782400LL).toISOString(), "2000-02-29T00:00:00Z");
	TEST_EQ(Date(951868800LL).toISOString(), "2000-03-01T00:00:00Z");
	TEST_EQ(Date(-2203891200LL).toISOString(), "1900-03-01T00:00:00Z");
	TEST_EQ(Date(4107542400LL).toISOString(), "2100-03-01T00:00:00Z");
	TEST_EQ(Date(dateValue(1900,3,1)).timestamp-Date(dateValue(1900,2,28)).timestamp, 86400LL);
	TEST_EQ(Date(dateValue(2000,3,1)).timestamp-Date(dateValue(2000,2,28)).timestamp, 172800LL);
	TEST_EQ(Date(dateValue(2100,3,1)).timestamp-Date(dateValue(2100,2,28)).timestamp, 86400LL);

	NextTest("Date every month boundary across a complete Gregorian cycle");
	const int lengths[]={31,28,31,30,31,30,31,31,30,31,30,31};
	for(int year=1800; year<2200; ++year){
		for(int month=1; month<=12; ++month){
			int last=lengths[month-1];
			if(month==2 && year%4==0 && (year%100!=0 || year%400==0)) ++last;
			Date start(dateValue(year,month,1)), end(dateValue(year,month,last,23,59,59));
			TEST_EQ(start.getYear(), (long long)year); TEST_EQ(start.getMonth(), month); TEST_EQ(start.getDay(), 1);
			TEST_EQ(end.getYear(), (long long)year); TEST_EQ(end.getMonth(), month); TEST_EQ(end.getDate(), last);
			TEST_EQ(end.timestamp-start.timestamp, last*86400LL-1);
			Date next(end.timestamp+1);
			TEST_EQ(next.getYear(), (long long)(month==12?year+1:year));
			TEST_EQ(next.getMonth(), month==12?1:month+1); TEST_EQ(next.getDay(), 1);
		}
	}

	NextTest("Date IMF month names and valid string round trips");
	const char* months[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	for(int month=1; month<=12; ++month){
		Date value(dateValue(2001,month,15,12,34,56));
		String expectedMonth=" "; expectedMonth+=months[month-1]; expectedMonth+=" ";
		TEST_NE(value.toIMFDate().indexOf(expectedMonth), (size_t)-1);
		TEST_EQ(Date(value.toIMFDate()).timestamp, value.timestamp);
		TEST_EQ(Date(value.toISOString()).timestamp, value.timestamp);
		TEST_EQ(Date(value.toDirectDate()).timestamp, value.timestamp);
		TEST_EQ(Date(value.toReverseDate()).timestamp, value.timestamp);
	}

	NextTest("Date parser Array<String> intermediates retain ownership across moves");
	Array<String> retained;
	retained.push("old destination");
	{
		Array<String> fields=String("2026-02-02").split("-");
		move(retained, fields);
		TEST_EQ(fields.size(), 0U); TEST_EQ(retained.size(), 3U);
		TEST_EQ(retained[0], "2026"); TEST_EQ(retained[1], "02"); TEST_EQ(retained[2], "02");
		fields.push("reused source"); TEST_EQ(fields[0], "reused source");
	}
	TEST_EQ(retained.size(), 3U); TEST_EQ(retained[0], "2026");
	Array<String> stolen;
	stolen.push("replace me"); stolen.steal(retained);
	TEST_EQ(retained.size(), 0U); TEST_EQ(stolen.size(), 3U); TEST_EQ(stolen[2], "02");
	retained.push("independent"); TEST_EQ(retained[0], "independent");
	TEST_EQ(stolen[0], "2026");
	#if __cplusplus >= 201103L
	Array<String> emptySource;
	Array<String> emptyMoved((Array<String>&&)emptySource);
	TEST_EQ(emptySource.size(), 0U); TEST_EQ(emptyMoved.size(), 0U);
	emptySource.push("source"); emptyMoved.push("destination");
	TEST_EQ(emptySource[0], "source"); TEST_EQ(emptyMoved[0], "destination");
	Array<String> populatedMoved((Array<String>&&)stolen);
	TEST_EQ(stolen.size(), 0U); TEST_EQ(populatedMoved.size(), 3U);
	TEST_EQ(populatedMoved[0], "2026"); TEST_EQ(populatedMoved[2], "02");
	stolen.push("reused moved-from"); TEST_EQ(stolen[0], "reused moved-from");
	TEST_EQ(populatedMoved[0], "2026");
	#endif
}
