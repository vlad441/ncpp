namespace ncpp {
    struct Date { long long timestamp; static char utc;
        struct DValue { long long year; char month, day, hour, minute; float seconds; };

        Date(); ~Date();
        Date(long long msecs, char type='s');
        Date(const CString& dateStr);
        Date(const DValue& dv);
        static long long now();

        long long getYear() const;
        int getMonth() const;
        int getDay() const;
        int getDate() const;
        
        int getDayWeek() const;

        String toDirectDate(char sp1 = '.', bool toshort = false) const;
        String toISOString() const;
        String toIMFDate() const;
        String toString() const;

        static DValue parseTimestamp(long long timestamp);
        static DValue parseDate(const String& dateStr);
        static long long toTimestamp(const DValue& dv);
    };

}