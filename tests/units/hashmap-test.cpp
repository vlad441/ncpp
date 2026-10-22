// Coverage for the complete public unit documented in docs/ru/base-HashMap.md.
// Keep the original runner and its failure accounting; do not depend on hash order.
void HashTestName(const char* container, const char* operation){
	String name(container); name += "."; name += operation; NextTest(name);
}

template <typename Table>
void HashContainer_test(const char* name, bool isSet){
	HashTestName(name, "construct/empty/size/begin/end");
	Table table; const Table& constant = table;
	TEST_EQ(table.empty(), true); TEST_EQ(table.size(), (size_t)0);
	TEST_EQ(table.begin(), table.end()); TEST_EQ(constant.begin(), constant.end());
	TEST_EQ(table.find(7), table.end()); TEST_EQ(constant.find(7), constant.end());
	TEST_EQ(table.has(7), false);
	TEST_EQ(table.keys().size(), (size_t)0);

	HashTestName(name, "insert explicit/default/duplicate");
	Pair<typename Table::Iter, bool> result = table.insert(0, 40);
	TEST_EQ(result.second, true); TEST_NE(result.first, table.end());
	if(result.first != table.end()) TEST_EQ(result.first->value(), isSet ? 0 : 40);
	TEST_EQ(table.empty(), false); TEST_EQ(table.size(), (size_t)1);
	result = table.insert(0, 99); TEST_EQ(result.second, false);
	TEST_EQ(table.size(), (size_t)1); TEST_EQ(constant.at(0), isSet ? 0 : 40);
	result = table.insert(8); TEST_EQ(result.second, true);
	if(result.first != table.end()) TEST_EQ(result.first->value(), isSet ? 8 : 0);
	table.insert(16, 160); // Same initial bucket: exercise linear probing.
	TEST_EQ(table.has(16), true); TEST_EQ(table.has(24), false);

	HashTestName(name, "find mutable/const/at missing");
	typename Table::Iter found = table.find(16);
	TEST_NE(found, table.end());
	if(found != table.end()) TEST_EQ(found->value(), isSet ? 16 : 160);
	typename Table::ConstIter cfound = constant.find(16);
	TEST_NE(cfound, constant.end());
	if(cfound != constant.end()) TEST_EQ((*cfound).value(), isSet ? 16 : 160);
	TEST_EQ(table.find(32), table.end()); TEST_EQ(constant.find(32), constant.end());
	// at() explicitly falls back to the FIRST element, but needs a nonempty table.
	TEST_EQ(constant.at(999), constant.begin()->value());
	TEST_EQ(table.size(), (size_t)3);

	HashTestName(name, "iterator prefix/postfix/conversion");
	typename Table::Iter it = table.begin();
	typename Table::ConstIter converted = it;
	TEST_EQ(converted, constant.begin()); TEST_EQ((*it).key(), it->key());
	typename Table::Iter previous = it++;
	TEST_NE(previous, it); TEST_EQ(previous, table.begin());
	typename Table::Iter& advanced = ++it; TEST_EQ(&advanced, &it);
	TEST_NE(it, table.end()); ++it; TEST_EQ(it, table.end());
	typename Table::Iter last = table.end();
	typename Table::Iter& reversed = --last; TEST_EQ(&reversed, &last);
	TEST_NE(last, table.end());
	typename Table::Iter oldLast = last--; TEST_NE(oldLast, last);
	--last; TEST_EQ(last, table.begin());
	typename Table::ConstIter clast = constant.end();
	--clast; TEST_EQ(clast->key(), oldLast->key());
	typename Table::ConstIter coldLast = clast--; TEST_NE(coldLast, clast);
	--clast; TEST_EQ(clast, constant.begin());
	Table other; TEST_NE(table.end(), other.end());
	size_t visited = 0; int sum = 0;
	for(typename Table::ConstIter scan = constant.begin(); scan != constant.end(); ++scan){
		++visited; sum += scan->key();
	}
	TEST_EQ(visited, (size_t)3); TEST_EQ(sum, 24);

	HashTestName(name, "keys/copy construction/copy assignment");
	Array<int> keys = table.keys(); TEST_EQ(keys.size(), (size_t)3);
	TEST_NE(keys.indexOf(0), NPOS); TEST_NE(keys.indexOf(8), NPOS);
	TEST_NE(keys.indexOf(16), NPOS); TEST_EQ(keys.indexOf(999), NPOS);
	Table copy(table); TEST_EQ(copy.size(), table.size());
	TEST_EQ(copy.erase(8), true); TEST_EQ(copy.has(8), false);
	TEST_EQ(table.has(8), true);
	Table assigned; assigned.insert(99); assigned = table;
	TEST_EQ(assigned.has(99), false); TEST_EQ(assigned.size(), (size_t)3);
	assigned.clear(); TEST_EQ(table.size(), (size_t)3);
	Table emptyCopy(assigned); TEST_EQ(emptyCopy.empty(), true);

	HashTestName(name, "erase key/iterator/collision chain");
	TEST_EQ(table.erase(8), true); TEST_EQ(table.erase(8), false);
	TEST_EQ(table.has(0), true); TEST_EQ(table.has(16), true);
	TEST_EQ(table.size(), (size_t)2);
	TEST_EQ(table.erase(table.end()), table.end());
	other.insert(5); TEST_EQ(table.erase(other.begin()), table.end());
	TEST_EQ(table.size(), (size_t)2);
	typename Table::Iter next = table.erase(table.find(0));
	TEST_EQ(table.has(0), false); TEST_EQ(table.has(16), true);
	TEST_NE(next, table.end());
	if(next != table.end()) TEST_EQ(next->key(), 16);
	table.erase(next); TEST_EQ(table.empty(), true);
	TEST_EQ(table.erase(100), false);

	HashTestName(name, "operator[] existing/default insertion");
	table.insert(4, 40); TEST_EQ(table[4], isSet ? 4 : 40);
	TEST_EQ(table[12], isSet ? 12 : 0); TEST_EQ(table.size(), (size_t)2);
	TEST_EQ(table[12], isSet ? 12 : 0); TEST_EQ(table.size(), (size_t)2);
	if(!isSet){ table[4] = 41; TEST_EQ(constant.at(4), 41); }
	TEST_EQ(table.has(12), true);

	HashTestName(name, "assign one/multiple/empty/self");
	Table source; source.insert(4, 400); source.insert(7, 70);
	Table& merged = Table::assign(table, source); TEST_EQ(&merged, &table);
	TEST_EQ(table.at(4), isSet ? 4 : 400); TEST_EQ(table.has(7), true);
	Table emptySource; Table::assign(table, emptySource);
	TEST_EQ(table.size(), (size_t)3);
	Table::assign(table, table); TEST_EQ(table.size(), (size_t)3);
	Array<Table> sources; Table::assign(table, sources);
	TEST_EQ(table.size(), (size_t)3);
	Table source2; source2.insert(4, 444); source2.insert(9, 90);
	sources.push(source); sources.push(source2);
	Table& multiMerged = Table::assign(table, sources); TEST_EQ(&multiMerged, &table);
	TEST_EQ(table.at(4), isSet ? 4 : 444); TEST_EQ(table.has(9), true);
	TEST_EQ(table.size(), (size_t)4);

	HashTestName(name, "clear/reuse/debug/type");
	TEST_EQ(table.toString(), table.cout()); TEST_EQ(table.type(), "map");
	TEST_NE(table.cout(), "{}");
	table.clear(); TEST_EQ(table.size(), (size_t)0); TEST_EQ(table.empty(), true);
	TEST_EQ(table.begin(), table.end()); TEST_EQ(table.keys().empty(), true);
	TEST_EQ(table.cout(), "{}"); TEST_EQ(table.toString(), "{}");
	TEST_EQ(table.type(), "map"); TEST_EQ(table.has(4), false);
	table.clear(); TEST_EQ(table.empty(), true);
	table.insert(22, 220); TEST_EQ(table.size(), (size_t)1);
	TEST_EQ(table.at(22), isSet ? 22 : 220);

	HashTestName(name, "rehash and wrapped collision deletion");
	table.clear();
	for(int i = 0; i < 100; ++i) TEST_EQ(table.insert(i * 8 + 7, i + 1).second, true);
	TEST_EQ(table.size(), (size_t)100);
	for(int i = 0; i < 100; ++i){
		TEST_EQ(table.has(i * 8 + 7), true);
		TEST_EQ(table.at(i * 8 + 7), isSet ? i * 8 + 7 : i + 1);
	}
	for(int i = 0; i < 100; i += 2) TEST_EQ(table.erase(i * 8 + 7), true);
	TEST_EQ(table.size(), (size_t)50);
	for(int i = 0; i < 100; ++i) TEST_EQ(table.has(i * 8 + 7), i % 2 == 1);
	table.clear(); table.insert(7); table.insert(15); table.insert(23);
	TEST_EQ(table.erase(7), true); TEST_EQ(table.has(15), true);
	TEST_EQ(table.has(23), true); TEST_EQ(table.has(7), false);

	HashTestName(name, "deterministic mixed operations against reference model");
	table.clear(); bool present[31] = {false}; int values[31] = {0};
	for(int step = 0; step < 350; ++step){
		int index = (step * 17) % 31, key = index * 8, value = step + 10;
		switch(step % 4){
			case 0: {
				bool fresh = !present[index];
				TEST_EQ(table.insert(key, value).second, fresh);
				if(fresh){ present[index] = true; values[index] = isSet ? key : value; }
				break;
			}
			case 1: TEST_EQ(table.erase(key), present[index]); present[index] = false; break;
			case 2:
				if(!present[index]){ present[index] = true; values[index] = isSet ? key : 0; }
				TEST_EQ(table[key], values[index]);
				if(!isSet){ table[key] = value; values[index] = value; }
				break;
			default: TEST_EQ(table.find(key) != table.end(), present[index]); break;
		}
		size_t expected = 0;
		for(int j = 0; j < 31; ++j){
			TEST_EQ(table.has(j * 8), present[j]);
			if(present[j]){ ++expected; TEST_EQ(table.at(j * 8), values[j]); }
		}
		TEST_EQ(table.size(), expected);
		if(step % 67 == 66){ table.clear(); for(int j = 0; j < 31; ++j) present[j] = false; }
	}
}

struct HashTrackedValue {
	static int alive; int value;
	HashTrackedValue(int v = 0) : value(v){ ++alive; }
	HashTrackedValue(const HashTrackedValue& other) : value(other.value){ ++alive; }
	~HashTrackedValue(){ --alive; }
	HashTrackedValue& operator=(const HashTrackedValue& other){ value = other.value; return *this; }
};
int HashTrackedValue::alive = 0;

void HashMap_test(){
	NextTest("hash/Fnv1a published vectors and length-aware binary inputs");
	TEST_EQ(Fnv1a("", 0), 2166136261U); TEST_EQ(Fnv1a("a", 1), 0xe40c292cU);
	TEST_EQ(Fnv1a("foobar", 6), 0xbf9cf968U);
	TEST_EQ(Fnv1a_64("", 0), 14695981039346656037ULL);
	TEST_EQ(Fnv1a_64("a", 1), 0xaf63dc4c8601ec8cULL);
	TEST_EQ(Fnv1a_64("foobar", 6), 0x85944171f73967e8ULL);
	const char binary[] = {'a', 0, 'b'};
	TEST_EQ(Fnv1a(binary, 3), 0x10f3abd2U);
	TEST_EQ(Fnv1a_64(binary, 3), 0xe5d29919042666b2ULL);
	TEST_EQ(ncpp::hash("hello"), 0x4f9f2cabU);
	TEST_EQ(ncpp::hash(String("hello")), 0x4f9f2cabU);
	TEST_EQ(ncpp::hash(CString("hello")), 0x4f9f2cabU);
	TEST_EQ(ncpp::hash(Buffer("hello")), 0x4f9f2cabU);
	TEST_EQ(ncpp::hash(String(binary, 3)), 0x10f3abd2U);
	TEST_EQ(ncpp::hash(CString(binary, 3)), 0x10f3abd2U);
	TEST_EQ(ncpp::hash(Buffer(binary, 3)), 0x10f3abd2U);
	TEST_NE(ncpp::hash(String(binary, 3)), ncpp::hash("a"));
	TEST_EQ(ncpp::hash(String()), 2166136261U);
	TEST_EQ(ncpp::hash(CString()), 2166136261U);
	TEST_EQ(ncpp::hash(Buffer()), 2166136261U);
	TEST_EQ(ncpp::hash(0), 0U); TEST_EQ(ncpp::hash(-1), 0xffffffffU);
	int number = 0; int* pointer = &number; int* nullPointer = NULL;
	TEST_EQ(ncpp::hash(pointer), ncpp::hash(&number));
	TEST_EQ(ncpp::hash(nullPointer), 0U);

	HashContainer_test<HashMap<int, int> >("HashMap", false);
	HashContainer_test<HashSet<int> >("HashSet", true);

	NextTest("HashMap/HashSet toJSON empty and nonempty objects");
	HashMap<String, int> numbers; TEST_EQ(numbers.toJSON(), "{}");
	numbers.insert("answer", 42); TEST_EQ(numbers.toJSON(), "{\"answer\":42}");
	numbers.clear(); TEST_EQ(numbers.toJSON(), "{}");
	HashSet<int> set; TEST_EQ(set.toJSON(), "{}");
	set.insert(7); TEST_EQ(set.toJSON(), "{\"7\":7}");
	numbers.insert("other", -2);
	String multiple = numbers.toJSON();
	TEST_EQ(multiple == "{\"other\":-2}" , true);
	numbers.insert("answer", 42); multiple = numbers.toJSON();
	TEST_EQ(multiple == "{\"answer\":42,\"other\":-2}" ||
		multiple == "{\"other\":-2,\"answer\":42}", true);

	NextTest("HashMap toJSON string escaping and embedded NUL");
	StringMap strings; strings.insert("a\"b\\c", "line\n\t\r\b\f\"\\");
	TEST_EQ(strings.toJSON(), "{\"a\\\"b\\\\c\":\"line\\n\\t\\r\\b\\f\\\"\\\\\"}");
	strings.clear(); strings.insert(String(binary, 3), String("\x01\x1f", 2));
	TEST_EQ(strings.toJSON(), "{\"a\\u0000b\":\"\\u0001\\u001f\"}");
	HashSet<String> stringSet; stringSet.insert("x\"y");
	TEST_EQ(stringSet.toJSON(), "{\"x\\\"y\":\"x\\\"y\"}");

	NextTest("HashMap toJSON native booleans and fractional values");
	HashMap<String, bool> flags; flags.insert("ok", true);
	TEST_EQ(flags.toJSON(), "{\"ok\":true}"); flags["ok"] = false;
	TEST_EQ(flags.toJSON(), "{\"ok\":false}");
	DoubleMap decimals; decimals.insert("value", 1.25);
	TEST_EQ(decimals.toJSON(), "{\"value\":1.25}");
	decimals["value"] = 1e30;
	TEST_EQ(decimals.toJSON(), "{\"value\":1e+30}");
	HashMap<String, float> floats; floats.insert("value", -1.25f);
	TEST_EQ(floats.toJSON(), "{\"value\":-1.25}");
	floats["value"] = 0.0f; TEST_EQ(floats.toJSON(), "{\"value\":0}");
	union { unsigned long long bits; double number; } nonfinite;
	nonfinite.bits = D_NAN; decimals["value"] = nonfinite.number;
	TEST_EQ(decimals.toJSON(), "{\"value\":null}");
	nonfinite.bits = D_INF; decimals["value"] = nonfinite.number;
	TEST_EQ(decimals.toJSON(), "{\"value\":null}");
	nonfinite.bits = D_NINF; decimals["value"] = nonfinite.number;
	TEST_EQ(decimals.toJSON(), "{\"value\":null}");

	NextTest("HashMap toJSON integer limits, CString and null C string");
	ULLMap maximum; maximum.insert("value", ULLONG_MAX);
	TEST_EQ(maximum.toJSON(), "{\"value\":18446744073709551615}");
	LLMap minimum; minimum.insert("value", LLONG_MIN);
	TEST_EQ(minimum.toJSON(), "{\"value\":-9223372036854775808}");
	HashMap<String, CString> views; views.insert("view", CString("a\"b"));
	TEST_EQ(views.toJSON(), "{\"view\":\"a\\\"b\"}");
	HashMap<String, const char*> pointers; pointers.insert("value", (const char*)NULL);
	TEST_EQ(pointers.toJSON(), "{\"value\":null}");
	pointers["value"] = "text";
	TEST_EQ(pointers.toJSON(), "{\"value\":\"text\"}");
	char mutableText[] = "a\"b";
	HashMap<String, char*> mutablePointers; mutablePointers.insert("value", mutableText);
	TEST_EQ(mutablePointers.toJSON(), "{\"value\":\"a\\\"b\"}");
	mutablePointers["value"] = NULL; TEST_EQ(mutablePointers.toJSON(), "{\"value\":null}");
	HashMap<String, long> longs; longs.insert("value", LONG_MIN);
	TEST_EQ(longs.toJSON(), sizeof(long) == 8 ? "{\"value\":-9223372036854775808}" :
		"{\"value\":-2147483648}");

	NextTest("HashMap/HashSet string-key assignment and missing at fallback");
	StringMap left, right; left.insert("old", "keep"); right.insert("new", "value");
	StringMap::assign(left, right); TEST_EQ(left.at("old"), "keep");
	TEST_EQ(left.at("new"), "value"); TEST_EQ(left.has("missing"), false);
	TEST_EQ(left.at("missing"), left.begin()->value());
	HashSet<String> setLeft, setRight; setLeft.insert("old"); setRight.insert("new");
	HashSet<String>::assign(setLeft, setRight); TEST_EQ(setLeft.has("old"), true);
	TEST_EQ(setLeft.has("new"), true); TEST_EQ(setLeft.has("missing"), false);
	TEST_EQ(setLeft.at("missing"), setLeft.begin()->value());

	NextTest("HashMap value lifetime through rehash/copy/erase/clear");
	int baseline = HashTrackedValue::alive;
	{
		HashMap<int, HashTrackedValue> tracked;
		for(int i = 0; i < 40; ++i) tracked.insert(i, HashTrackedValue(i));
		HashMap<int, HashTrackedValue> copy(tracked);
		TEST_EQ(copy.at(17).value, 17); copy.erase(17);
		TEST_EQ(tracked.has(17), true); tracked.clear();
		TEST_EQ(tracked.empty(), true); copy.clear();
	}
	TEST_EQ(HashTrackedValue::alive, baseline);
}
