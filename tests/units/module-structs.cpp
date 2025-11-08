/*void bigint_matan_test_old(){ // 26959946667150639794667015087019625481340285887712943950607281029120 / 65537 = 411369862324345633682759587515748744699029340490302332279586814
    NextTest("Buffer::Math::divide_single(Buffer&, unsigned int, bool)");
	BigInt r1("26959946667150639794667015087019625481340285887712943950607281029120"); Buffer::Math::divide_single(r1, 65537, true);
	cons << "--- divide_single: " << r1.toIntString() << "\n"; 
	TEST_EQ(r1.toIntString(), "411369862324345633682759587515748744699029340490302332279586814");
	
	NextTest("Buffer::Math::binary_divmod(const Buffer& a, const Buffer& b, bool)"); rsetCons(); Pair<Buffer, Buffer> qr;
	qr.first = Buffer::Math::divide(BigInt("26959946667150639794667015087019625481340285887712943950607281029120"), BigInt("65537"));
	TEST_EQ(qr.first.toIntString(), "411369862324345633682759587515748744699029340490302332279586814");
	cons << "---- Math::divide: " << qr.first.toIntString() << "\n";
	
	qr=Buffer::Math::binary_divmod(BigInt("180"), BigInt("7")); TEST_EQ(qr.first.toIntString(), "25"); TEST_EQ(qr.second.toIntString(), "5");
	cons << "---Buffer::Math::divide: 180/7 = " << qr.first.toIntString() << " (Correct: ~25.71 - R: 5) | R: " << qr.second.toIntString() << "\n";
	
	qr=Buffer::Math::binary_divmod(BigInt("500"), BigInt("80")); TEST_EQ(qr.first.toIntString(), "6"); TEST_EQ(qr.second.toIntString(), "20");
	cons << "---Buffer::Math::divide: 500/80 = " << qr.first.toIntString() << " (Correct: 6,25 - R: 20) | R: " << qr.second.toIntString() << "\n";
	
	qr=Buffer::Math::binary_divmod(BigInt("23242"), BigInt("123")); TEST_EQ(qr.first.toIntString(), "188"); TEST_EQ(qr.second.toIntString(), "118");
	cons << "---Buffer::Math::divide: 23242/123 = " << qr.first.toIntString() << " (Correct: ~188,95 - R: 118) | R: " << qr.second.toIntString() << "\n";
	
	qr=Buffer::Math::binary_divmod(BigInt("1892642"), BigInt("1623")); TEST_EQ(qr.first.toIntString(), "1166"); TEST_EQ(qr.second.toIntString(), "224");
	cons << "---Buffer::Math::divide: 1892642/1623 = " << qr.first.toIntString() << " (Correct: ~1166,13 - R: 224) | R: " << qr.second.toIntString() << "\n";
	
	qr=Buffer::Math::binary_divmod(BigInt("162305008000"), BigInt("12305008000")); TEST_EQ(qr.first.toIntString(), "13"); TEST_EQ(qr.second.toIntString(), "2339904000");
	cons << "---Buffer::Math::divide: 162305008000/12305008000 = " << qr.first.toIntString() << " (Correct: ~13.19 - R: 2339904000) | R: " << qr.second.toIntString() << "\n";
}*/

void bigint_powMod_test(){
	//cons << "long long multiply: 318269061 * 3242605871 = " << 318269061*3242605871 << "\n";
	//cons << "BigInt multiply:    318269061 * 3242605871 = " << BigInt("318269061")*BigInt("3242605871") << "\n"; 
	NextTest("BigInt::powMod(const Buffer& a, const Buffer& b, bool)"); rsetCons(); BigInt powmod; double pow_time=0;
	
	pow_time = GetTimestamp('u');
	powmod = BigInt::powMod(BigInt("5784410983399090098979"), 65537, BigInt("728441098339909009897989899899"));
	TEST_EQ(powmod.toString(), "436974669932865908801807593904");
	cons << "--- powMod(~96 bits) = " << powmod.toString() << "\n";
	cons << "--- == Time: " << dtos((GetTimestamp('u')-pow_time)/1000.0, 3) << " msec.\n";
	
	pow_time = GetTimestamp('u');
	powmod = BigInt::powMod(BigInt("448441098339"), 65537, BigInt("8337081413648785545319633427184794663442231257812474053748924357873034396348443147463459176658193871428912054316060197154657584308858711521584149869800409"));
	TEST_EQ(powmod.toString(), "8235268947183623068183478460196304961213547175072910579233784236435658850672398906533320054125179477276517696070551883850668816422827485613146966261594282");
	cons << "--- Encrypt powMod(512 bits) = " << powmod.toString() << "\n";
	cons << "--- == Time: " << dtos((GetTimestamp('u')-pow_time)/1000.0, 3) << " msec.\n";
	
	pow_time = GetTimestamp('u');
	powmod = BigInt::powMod(BigInt("8235268947183623068183478460196304961213547175072910579233784236435658850672398906533320054125179477276517696070551883850668816422827485613146966261594282"), BigInt("1255199083090049391575275386820153027208820907591676175112388980867803384176392806418198950849985557076510219996257973803858569120555626307266334391903437"), BigInt("8337081413648785545319633427184794663442231257812474053748924357873034396348443147463459176658193871428912054316060197154657584308858711521584149869800409"));
	TEST_EQ(powmod.toString(), "448441098339");
	cons << "--- Decrypt powMod(512 bits) = " << powmod.toString() << "\n";
	cons << "--- == Time: " << dtos((GetTimestamp('u')-pow_time)/1000.0, 3) << " msec.\n";
}

void bigint_test(){
	//NextTest("Buffer::removeLeadingZeros(Buffer& buff)"); Buffer z_buff=Buffer::fromHex("0000ff01"); Buffer::removeLeadingZeros(z_buff);
	
	NextTest("BigInt::increment(BigInt&, value=1, bool)"); BigInt bint=Buffer(8, 0xff);
	BigInt::increment(bint, 1, false); TEST_EQ(bint.toHex(), "00");
	BigInt::increment(bint, 2, false); TEST_EQ(bint.toHex(), "02");
	bint=Buffer(8, 0xff); BigInt::increment(bint, 4, false); TEST_EQ(bint.toHex(), "03");
	bint = BigInt::fromHex("fffffffffffffffd"); BigInt::increment(bint, 0xFFFFFFFA); TEST_EQ(bint.toString(), "18446744078004518903");
	
	NextTest("BigInt::decrement(Buffer&, value=1, bool)"); bint=Buffer(8, 0);
	BigInt::decrement(bint, 1, false); TEST_EQ(bint.toHex(), "ffffffffffffffff");
	BigInt::decrement(bint, 2, false); TEST_EQ(bint.toHex(), "fffffffffffffffd");
	bint=Buffer(8, 0); BigInt::decrement(bint, 4, false); TEST_EQ(bint.toHex(), "fffffffffffffffc");
	bint = BigInt::fromHex("fffffffffffffffd"); BigInt::decrement(bint, 0xFFFFFFFA); TEST_EQ(bint.toString(), "18446744069414584323");
	
	NextTest("BigInt::divide_single(BigInt&, unsigned int, bool)");
	BigInt r1("26959946667150639794667015087019625481340285887712943950607281029120"); BigInt::divide_single(r1, 65537);
	TEST_EQ(r1.toIntString(), "411369862324345633682759587515748744699029340490302332279586814");
	rsetCons(); cons << "-- BigInt::divide_single: " << r1.toIntString() << "\n";
	NextTest("BigInt::multiply_single(BigInt&, unsigned int, bool)");
	r1 = "411369862324345633682759587515748744699029340490302332279586814"; BigInt::multiply_single(r1, 65537);
	TEST_EQ(r1.toIntString(), "26959946667150639794667015087019625481340285887712943950607281029118");
	
	NextTest("BigInt::add(const BigInt& a, const BigInt& b, bool)");
	bint = BigInt::add("18446744078004518903", "22446744069414584323"); TEST_EQ(bint.toString(), "40893488147419103226");
	NextTest("BigInt::subtract(const BigInt& a, const BigInt& b, bool)");
	bint = BigInt::subtract("40893488147419103226", "18446744078004518903"); TEST_EQ(bint.toString(), "22446744069414584323");
	bint = BigInt::subtract("4342617250", "999888777"); TEST_EQ(bint.toString(), "3342728473");
	cons << "-- BigInt::subtract: " << bint.toString() << "\n";
	NextTest("BigInt::multiply(const BigInt& a, const BigInt& b, bool)");
	bint = BigInt::multiply("18446744078004518903", "22446744069414584323"); TEST_EQ(bint.toString(), "414069343232956538946247398967340957669");
	
	// 26959946667150639794667015087019625481340285887712943950607281029120 / 65537 = 411369862324345633682759587515748744699029340490302332279586814
	NextTest("BigInt::divmod(const Buffer& a, const Buffer& b, bool)"); rsetCons(); Pair<BigInt, BigInt> qr;
	cons << "---------- etalon: 411369862324345633682759587515748744699029340490302332279586814" << "\n";
	qr.first = BigInt::divide(BigInt("26959946667150639794667015087019625481340285887712943950607281029120"), BigInt("65537"));
	TEST_EQ(qr.first.toIntString(), "411369862324345633682759587515748744699029340490302332279586814");
	cons << "-- BigInt::divide: " << qr.first.toIntString() << "\n";
	
	qr=BigInt::divmod(BigInt("180"), BigInt("7")); TEST_EQ(qr.first.toIntString(), "25"); TEST_EQ(qr.second.toIntString(), "5");
	cons << "---BigInt::divmod: 180/7 = " << qr.first.toIntString() << " (Correct: ~25.71 - R: 5) | R: " << qr.second.toIntString() << "\n";
	
	qr=BigInt::divmod(BigInt("23242"), BigInt("123")); TEST_EQ(qr.first.toIntString(), "188"); TEST_EQ(qr.second.toIntString(), "118");
	cons << "---BigInt::divmod: 23242/123 = " << qr.first.toIntString() << " (Correct: ~188,95 - R: 118) | R: " << qr.second.toIntString() << "\n";
	
	qr=BigInt::divmod(BigInt("1892642"), BigInt("1623")); TEST_EQ(qr.first.toIntString(), "1166"); TEST_EQ(qr.second.toIntString(), "224");
	cons << "---BigInt::divmod: 1892642/1623 = " << qr.first.toIntString() << " (Correct: ~1166,13 - R: 224) | R: " << qr.second.toIntString() << "\n";
	
	qr=BigInt::divmod(BigInt("162305008000"), BigInt("12305008000")); TEST_EQ(qr.first.toIntString(), "13"); TEST_EQ(qr.second.toIntString(), "2339904000");
	cons << "---BigInt::divmod: 162305008000/12305008000 = " << qr.first.toIntString() << " (Correct: ~13.19 - R: 2339904000) | R: " << qr.second.toIntString() << "\n";
	
	qr=BigInt::divmod(BigInt("14499999989490968294177999999"), BigInt("22446744069414584323")); TEST_EQ(qr.first.toIntString(), "645973418"); TEST_EQ(qr.second.toIntString(), "473985");
	cons << "---BigInt::divmod: 14499999989490968294177999999/22446744069414584323 = " << qr.first.toIntString() << " (Correct: 645973418 - R: 473985) | R: " << qr.second.toIntString() << "\n";
	
	//26959946667150639794667015087019625481340285887712943950607281029120 / 22446744069414584323 = 1201062683468897443469154161044064935211125440901
	qr=BigInt::divmod(BigInt("26959946667150639794667015087019625481340285887712943950607281029120"), BigInt("22446744069414584323")); 
	TEST_EQ(qr.first.toIntString(), "1201062683468897443469154161044064935211125440901"); TEST_EQ(qr.second.toIntString(), "20868881454263434097");
	cons << "---BigInt::divmod: " << qr.first.toIntString() << " | R: " << qr.second.toIntString() << "\n";
	
	NextTest("BigInt::mod(const BigInt& a, const BigInt& b, bool)"); rsetCons(); BigInt modval;
	modval = BigInt::mod("1440000000000000000000980", 999888777); TEST_EQ(modval.toString(), "752126543");
	cons << "---mod: 1440000000000000000000980 % 999888777 = " << modval.toString()  << " (Correct R: 752126543)\n";
	modval = BigInt::mod("14499999989490968294177999999", "22446744069414584323"); TEST_EQ(modval.toString(), "473985");
	cons << "---mod: 14499999989490968294177999999 % 22446744069414584323 = " << modval.toString() << "\n";
	
	bigint_powMod_test();
}

#ifdef NCPP_INT128
void __int128_test(){
	NextTest("uint128_t::mul_64x64_128(uint64_t, uint64_t)"); uint128_t myval;
	myval = uint128_t::mul_64x64_128(0xFFFFFFFFFFFFFFFAULL, 0xFFFFFFFFFFFFFFFAULL);
	TEST_EQ(myval._toBuff().toIntString(), "340282366920938463242013678547253592100");
}
#else
void __int128_test(){};
#endif

void cbor_test(){ Buffer cbor; Object obj; obj["test"]="txt"; obj["double"]=12.334; obj["int"]=13; obj["buff"]=Buffer(3,4); obj["obj"]=Object(); obj["obj"]["buff"]=Buffer(3,1);
	obj["obj"]["deep"]=Object(); obj["obj"]["deep"]["state"]=true;
	cons << "obj: " << obj.cout(4) << "\n";
	cbor=CBOR::serialize(obj); cons << "cbor buff: " << cbor.cout(100) << "\n";
	obj=CBOR::parse(cbor); cons << "cbor parsed: " << obj.cout(4) << "\n"; 
	
	cbor=Buffer::from("0dff0904", "hex"); obj=CBOR::parse(cbor);
	cons << "== cbor buff 2: " << cbor << "\n"; 
	cons << "== cbor parsed 2: " << obj << "\n";	
}

void Structs_module_test(){
	bigint_test();
	__int128_test();
}