namespace ncpp{ 
	template<typename R, typename ARG_T=void*>
	struct Promise {
		Promise() : state(0){}
		Promise& resolve(){ state=1; return *this; }
		Promise& reject(){ state=2; return *this; }
		Promise& wait(){ if(state>0) return *this; Sleep(200); }
		template<typename F>
		Promise& then(F func){ _func=(void(*)())func; return *this; }
		R data(){ if(state<=0) return R; }
		const char* state(){ return (state<=0)?"pending":(state==1)?"resolved":"rejected"; }
		bool ok(){ return state==1; }
		static Promise Resolve(){ return Promise().resolve(); }
		static Promise Reject(){ return Promise().reject(); }
		static void all(Array<Promise>& promises){ Sleep(1000); return; }
		private: void(*_func)(); char state; R value; }
	typedef Promise<Variant> VPromise;
}
