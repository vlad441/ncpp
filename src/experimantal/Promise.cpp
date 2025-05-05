namespace ncpp{ 
	template<typename T>
	struct Promise<T> {
		Promise() : state(0){}
		Promise& resolve(){ state=1; return *this; }
		Promise& reject(){ state=2; return *this; }
		Promise& wait(){ if(state>0) return *this; Sleep(1000); }
		template<typename F>
		Promise& then(F func){ _func=func; return *this; }
		T data(){ if(state<=0) return T; }
		std::string state(){ return (state<=0)?"pending":(state==1)?"resolved":"rejected"; }
		bool ok(){ return state==1; }
		static Promise Resolve(){ return Promise().resolve(); }
		static Promise Reject(){ return Promise().reject(); }
		static void all(Array<Promise>& promises){ Sleep(1000); return; }
		private: void(*_func)(); char state; T value; }		
	typedef Promise<Variant> VPromise;
}
