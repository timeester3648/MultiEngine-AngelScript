#include "utils.h"
#include "../../../add_on/scriptarray/scriptarray.h"
#include "../../../add_on/weakref/weakref.h"
#include "../../../add_on/scriptdictionary/scriptdictionary.h"
#include "../../../add_on/scriptstdstring/scriptstdstring.h"
#include "../../../add_on/scripthandle/scripthandle.h"

namespace Test_Addon_ScriptArray
{

static const char* TESTNAME = "Test_Addon_ScriptArray";

static const char* script1 =
	"array<string> b;                                \n"
	"array<int> g_a(3);                              \n"
	"void TestArray()                                \n"
	"{                                               \n"
	"   array<string> a(5);                          \n"
	"   Assert(a.length() == 5);                     \n"
	"   a.resize(10);                                \n"
	"   a.resize(5);                                 \n"
	"   a[0] = \"Hello\";                            \n"
	"   Assert(a[0] == \"Hello\");                   \n"
	"   uint n = 0;                                  \n"
	"   Assert(a[n] == \"Hello\");                   \n"
	"   n++;                                         \n"
	"   Assert(a[n] == \"\");                        \n"
	"   b = a;                                       \n"
	"   Assert(b.length() == 5);                     \n"
	"   Assert(b[0] == \"Hello\");                   \n"
	"   b[0] = \"Goodbye\";                          \n"
	"   Assert(a[0] != \"Goodbye\");                 \n"
	"   array<int> ia = TestArray4();                \n"
	"   TestArray2(ia);                              \n"
	"   TestArray3(ia);                              \n"
	"   ia = array<int>(3);                          \n"
	"   Assert(ia.length() == 3);                    \n"
	"   ia[0] = 1;                                   \n"
	"   array<int> ib = ia;                          \n"
	"   Assert(ib.length() == ia.length());          \n"
	"   Assert(ib[0] == ia[0]);                      \n"
	"}                                               \n"
	"void TestArray2(array<int> &inout a)            \n"
	"{                                               \n"
	"   Assert(a[0] == 1);                           \n"
	"   Assert(a[1] == 2);                           \n"
	"   Assert(a[2] == 3);                           \n"
	"}                                               \n"
	"void TestArray3(array<int> a)                   \n"
	"{                                               \n"
	"   Assert(a[0] == 1);                           \n"
	"   Assert(a[1] == 2);                           \n"
	"   Assert(a[2] == 3);                           \n"
	"}                                               \n"
	"array<int> TestArray4()                         \n"
	"{                                               \n"
	"   array<int> ia(3);                            \n"
	"   ia[0] = 1;                                   \n"
	"   ia[1] = 2;                                   \n"
	"   ia[2] = 3;                                   \n"
	"   return ia;                                   \n"
	"}                                               \n";

static const char* script2 =
	"void TestArrayException()                       \n"
	"{                                               \n"
	"   array<string> a;                             \n"
	"   a[0] == \"Hello\";                           \n"
	"}                                               \n";

// Must be possible to declare array of arrays
static const char* script3 =
	"void TestArrayMulti()                           \n"
	"{                                               \n"
	"   array<array<int>> a(2);                      \n"
	"   array<int> b(2);                             \n"
	"   a[0] = b;                                    \n"
	"   a[1] = b;                                    \n"
	"                                                \n"
	"   a[0][0] = 0;                                 \n"
	"   a[0][1] = 1;                                 \n"
	"   a[1][0] = 2;                                 \n"
	"   a[1][1] = 3;                                 \n"
	"                                                \n"
	"   Assert(a[0][0] == 0);                        \n"
	"   Assert(a[0][1] == 1);                        \n"
	"   Assert(a[1][0] == 2);                        \n"
	"   Assert(a[1][1] == 3);                        \n"
	"}                                               \n";

static const char* script4 =
	"void TestArrayChar()                            \n"
	"{                                               \n"
	"   array<int8> a(2);                            \n"
	"   a[0] = 13;                                   \n"
	"   a[1] = 19;                                   \n"
	"                                                \n"
	"   int8 a0 = a[0];                              \n"
	"   int8 a1 = a[1];                              \n"
	"   Assert(a[0] == 13);                          \n"
	"   Assert(a[1] == 19);                          \n"
	"}                                               \n";

static const char* script5 =
	"array<int> g = {1,2,3};                         \n"
	"void TestArrayInitList()                        \n"
	"{                                               \n"
	"   Assert(g.length() == 3);                     \n"
	"   Assert(g[2] == 3);                           \n"
	"   array<int> a = {,2,};                        \n"
	"   Assert(a.length() == 3);                     \n"
	"   Assert(a[1] == 2);                           \n"
	"   array<string> b = {\"test\", \"3\"};         \n"
	"   Assert(b.length() == 2);                     \n"
	"   Assert(b[0] == \"test\");                    \n"
	"   Assert(b[1] == \"3\");                       \n"
	"   array<array<int>> c = {,{23},{23,4},};       \n"
	"   Assert(c.length() == 4);                     \n"
	"   Assert(c[2].length() == 2);                  \n"
	"   Assert(c[2][1] == 4);                        \n"
	"   const array<int> d = {0,1,2};                \n"
	"   Assert(d.length() == 3);                     \n"
	"   Assert(d[2] == 2);                           \n"
	"}                                               \n";

static const char* script6 =
	"void Test()                                     \n"
	"{                                               \n"
	"   array<int>@ e = {2,5};                       \n"
	"   array<int> f = {,{23}};                      \n"
	"}                                               \n";

static const char* script7 =
	"class TestC                                     \n"
	"{                                               \n"
	"  TestC() {count++; s = \"test\";}              \n"
	"  string s;                                     \n"
	"}                                               \n"
	"int count = 0;                                  \n"
	"void Test()                                     \n"
	"{                                               \n"
	"  TestC t;                                      \n"
	"  Assert(count == 1);                           \n"
	"  array<TestC> arr(5);                          \n"
	"  Assert(count == 6);                           \n"
	"}                                               \n";

bool Test2();

std::string g_printBuf;
void printStr(asIScriptGeneric *gen)
{
	std::string *val = (std::string *)gen->GetArgAddress(0);
	g_printBuf += *val;
}

class ClassExceptionInConstructor
{
public:
	ClassExceptionInConstructor()
	{
		if( !strstr(asGetLibraryOptions(), "AS_NO_EXCEPTIONS") )
			throw std::exception();
		else
		{
			asIScriptContext *ctx = asGetActiveContext();
			if( ctx ) ctx->SetException("Caught an exception from the application");
		}
	}
	~ClassExceptionInConstructor() {}
	ClassExceptionInConstructor &operator=(const ClassExceptionInConstructor &) { return *this; }

	static void Construct(void *mem) { new(mem) ClassExceptionInConstructor(); }
	static void Destruct(ClassExceptionInConstructor *mem) { mem->~ClassExceptionInConstructor(); }
};

CScriptArray *CreateArrayOfStrings()
{
	asIScriptContext *ctx = asGetActiveContext();
	if( ctx )
	{
		asIScriptEngine* engine = ctx->GetEngine();
		asITypeInfo* t = engine->GetTypeInfoByDecl("array<string@>");
		CScriptArray* arr = CScriptArray::Create(t, 3);
		for( asUINT i = 0; i < arr->GetSize(); i++ )
		{
			CScriptString** p = static_cast<CScriptString**>(arr->At(i));
			*p = new CScriptString("test");
		}
		return arr;
	}
	return 0;
}

static std::stringstream printResult;
static void print(asIScriptGeneric *gen) 
{
	void* ptr = *(void**)gen->GetAddressOfArg(0);
	int typeId = gen->GetArgTypeId(0);

	asIScriptContext* ctx = asGetActiveContext();
	asIScriptEngine* engine = ctx->GetEngine();
	asITypeInfo* ti = engine->GetTypeInfoById(typeId);
	asIScriptFunction* func = ti->GetMethodByName("get_value");
	ctx->PushState();
	ctx->Prepare(func);
	if (typeId & asTYPEID_OBJHANDLE)
		ptr = *(void**)ptr;
	ctx->SetObject(ptr); //##Crash!
	ctx->Execute();
	printResult << ctx->GetReturnDWord() << "\n";
	ctx->PopState();
}

static bool testVal = false;
static bool called = false;

static float  t1 = 0;
static float  t2 = 0;
static double t3 = 0;
static float  t4 = 0;

template < class T >
void FillASArray(std::vector < T >& in, CScriptArray* out)
{
	out->Resize((asUINT)in.size());
	for (int i = 0; i < (int)in.size(); i++)
	{
		out->SetValue(i, &in[i]);
	}
}

template < class T >
void FillSTLVector(CScriptArray* in, std::vector < T >& out)
{
	out.resize(in->GetSize());
	for (int i = 0; i < (int)in->GetSize(); i++)
	{
		out[i] = *(T*)(in->At(i));
	}
}

void doCalculations(const std::string& /* geom_id */, const int& /* surf_indx */, const std::vector < double >& us, const std::vector < double >& ws, std::vector < double >& k1_out_vec, std::vector < double >& k2_out_vec, std::vector < double >& ka_out_vec, std::vector < double >& kg_out_vec)
{
	called = true;

	k1_out_vec.resize(0);
	k2_out_vec.resize(0);
	ka_out_vec.resize(0);
	kg_out_vec.resize(0);

	if (us.size() == ws.size())
	{
		k1_out_vec.resize(us.size());
		k2_out_vec.resize(us.size());
		ka_out_vec.resize(us.size());
		kg_out_vec.resize(us.size());

		for (unsigned int i = 0; i < us.size(); i++)
		{
			k1_out_vec[i] = us[i];
			k2_out_vec[i] = ws[i];
			ka_out_vec[i] = us[i] * ws[i];
			kg_out_vec[i] = us[i] / ws[i];

			// Store last calculated value for checking later.
			t1 = (float)k1_out_vec[i];
			t2 = (float)k2_out_vec[i];
			t3 = ka_out_vec[i];
			t4 = (float)kg_out_vec[i];
		}
	}

	double eps = 1e-4;
	testVal = (std::abs(t1 - 0.833333) < eps) &&
		(std::abs(t2 - 0.166667) < eps) &&
		(std::abs(t3 - 0.138889) < eps) &&
		(std::abs(t4 - 5.000000) < eps);
}


class TestClass
{
public:
	void cfunction(const std::string& geom_id, const int& surf_indx, CScriptArray* us, CScriptArray* ws, CScriptArray* k1s,
		CScriptArray* k2s, CScriptArray* kas, CScriptArray* kgs)
	{
		std::vector < double > in_us;
		FillSTLVector(us, in_us);

		std::vector < double > in_ws;
		FillSTLVector(ws, in_ws);

		std::vector < double > out_k1s;
		std::vector < double > out_k2s;
		std::vector < double > out_kas;
		std::vector < double > out_kgs;

		doCalculations(geom_id, surf_indx, in_us, in_ws, out_k1s, out_k2s, out_kas, out_kgs);

		FillASArray(out_k1s, k1s);
		FillASArray(out_k2s, k2s);
		FillASArray(out_kas, kas);
		FillASArray(out_kgs, kgs);
	}
};

bool Test()
{
	bool fail = false;
	fail = Test2() || fail;
	int r;
	COutStream out;
	CBufferedOutStream bout;
	asIScriptContext *ctx;
	asIScriptEngine *engine;

	// Allow registering a property with arrays of const handles that cannot be modified by the script
	// Reported by Tomasz
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		RegisterScriptArray(engine, true);

		engine->RegisterObjectType("foo", 0, asOBJ_REF);
		engine->RegisterObjectBehaviour("foo", asBEHAVE_ADDREF, "void f()", asFUNCTION(0), asCALL_GENERIC);
		engine->RegisterObjectBehaviour("foo", asBEHAVE_RELEASE, "void f()", asFUNCTION(0), asCALL_GENERIC);

		r = engine->RegisterGlobalProperty("const array<const foo @const> arr", (void*)1);
		if (r < 0)
			TEST_FAILED;

		asIScriptModule* mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test", "void func() { @arr[0] = null; }");
		r = mod->Build();
		if (r >= 0)
			TEST_FAILED;

		if (bout.buffer != "test (1, 1) : Info    : Compiling void func()\n"
						   "test (1, 23) : Error   : Reference is read-only\n")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test a case with double arrays
	// Reported by Rob McDonald
	SKIP_ON_MAX_PORT
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

		RegisterScriptArray(engine, true);
		RegisterStdString(engine);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		TestClass tc_instance;

		r = engine->RegisterGlobalFunction("void cfunction(const string & in geom_id, const int & in surf_indx, array<double>@+ us, array<double>@+ ws, array<double>@+ k1s, array<double>@+ k2s, array<double>@+ kas, array<double>@+ kgs)", asMETHOD(TestClass, cfunction), asCALL_THISCALL_ASGLOBAL, &tc_instance);
		assert(r >= 0);

		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);

		const char* script =
			"int n = 5;                                                       \n"
			"string geom_id = 'ABCDEF';                                       \n"
			"array<double> uvec, wvec;                                        \n"
			"uvec.resize( n );                                                \n"
			"wvec.resize( n );                                                \n"
			"for( int i = 0 ; i < n ; i++ )                                   \n"
			"{                                                                \n"
			"    uvec[i] = (i+1)*1.0/(n+1);                                   \n"
			"    wvec[i] = (n-i)*1.0/(n+1);                                   \n"
			"}                                                                \n"
			"array<double> k1vec, k2vec, kavec, kgvec;                        \n"
			"cfunction( geom_id, 0, uvec, wvec, k1vec, k2vec, kavec, kgvec ); \n"
			"assert( k1vec.length() == 5 && k2vec.length() == 5 && kavec.length() == 5 && kgvec.length() == 5 ); \n";

		ExecuteString(engine, script);
		if (!called)
		{
			// failure
			PRINTF("\n%s: cfunction not called from script\n\n", TESTNAME);
			TEST_FAILED;
		}
		else if (!testVal)
		{
			// failure
			PRINTF("\n%s: testVal is not of expected value. Got (%f, %f, %f, %f), expected (%f, %f, %f, %f)\n\n", TESTNAME, t1, t2, t3, t4, 0.833333, 0.166667, 0.138889, 5.000000);
			TEST_FAILED;
		}

		called = false;
		testVal = false;
		ExecuteString(engine, script);
		if (!called)
		{
			// failure
			PRINTF("\n%s: cfunction not called from script\n\n", TESTNAME);
			TEST_FAILED;
		}
		else if (!testVal)
		{
			// failure
			PRINTF("\n%s: testVal is not of expected value. Got (%f, %f, %f, %f), expected (%f, %f, %f, %f)\n\n", TESTNAME, t1, t2, t3, t4, 0.833333, 0.166667, 0.138889, 5.000000);
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test sort with callback as delegate
	// Reported by Patrick Jeeves
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		RegisterScriptArray(engine, false);

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		asIScriptModule* mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"class Test { \n"
			"  Test() { val = 0; } \n"
			"  Test(int a) { val = a; } \n"
			"  bool Less(const Test @const&in a, const Test @const&in b) { return a.val < b.val; } \n"
			"  int val; \n"
			"} \n"
			"void main() { \n"
			"  array<Test@> arr = {Test(3),Test(2),Test(1)}; \n"
			"  Test t; \n"
			"  arr.sort(array<Test@>::less(t.Less)); \n" // use delegate to compare the elements and sort
			"  assert( arr[0].val == 1 ); \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test sort with callback as delegate (attempt to use method not matching funcdef should give appropriate error)
	// Reported by Patrick Jeeves
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		RegisterScriptArray(engine, false);

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		asIScriptModule* mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"class Test { \n"
			"  Test() { val = 0; } \n"
			"  Test(int a) { val = a; } \n"
			"  bool Less(const Test @ a, const Test @ b) { return a.val < b.val; } \n"
			"  int val; \n"
			"} \n"
			"void main() { \n"
			"  array<Test@> arr = {Test(3),Test(2),Test(1)}; \n"
			"  Test t; \n"
			"  arr.sort(array<Test@>::less(t.Less)); \n" // Test::Less doesn't match the array::less funcdef
			"  assert( arr[0].val == 1 ); \n"
			"} \n");
		r = mod->Build();
		if (r >= 0)
			TEST_FAILED;

		if (bout.buffer != "test (7, 1) : Info    : Compiling void main()\n"
						   "test (10, 12) : Error   : Can't create delegate\n"
						   "test (10, 12) : Info    : No matching signatures to 'bool array<Test@>::less(const Test@const&in, const Test@const&in)'\n")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test array with uint64
	// https://www.gamedev.net/forums/topic/711196-cscriptarrayequals-doesnt-know-about-64-bit-types/
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		RegisterScriptArray(engine, false);
		RegisterStdString(engine);
		g_printBuf = "";
		engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(printStr), asCALL_GENERIC);

		asIScriptModule* mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"void main() { \n"
			"uint64 zero = 0;\n"
			"uint64 nonzero = 0x100000000; \n"
			"array<uint64> array_zero = { zero }; \n"
			"array<uint64> array_nonzero = { nonzero }; \n"
			"print('' + (zero == nonzero) + '\\n'); \n"             //false, good
			"print('' + (array_zero == array_nonzero) + '\\n'); \n" //true, but should be false
			"print('' + array_nonzero.find(zero) + '\\n'); \n"      //0, but should be -1
			"array<uint64> arr = { 0, 2, 4, 0x100000001, 0x100000003, 0x100000005 }; \n"
			"arr.sortAsc(); \n"
			"for (uint i = 0; i < arr.length(); ++i) \n"
			"	print(formatInt(arr[i], 'H') + '\\n'); \n" // 0, 100000001, 2, 100000003, 4, 100000005
			"}\n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		if (g_printBuf != "false\n"
						  "false\n"
						  "-1\n"
						  "0\n""2\n""4\n""100000001\n""100000003\n""100000005\n")
		{
			PRINTF("%s", g_printBuf.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test sorting array of handles where objects are garbage collectable
	// https://www.gamedev.net/forums/topic/709910-objects-with-handle-member-can-get-garbage-collected-after-array-sort/
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		RegisterScriptArray(engine, false);
		RegisterStdString(engine);
		g_printBuf = "";
		engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(printStr), asCALL_GENERIC);

		asIScriptModule* mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"class Bar {} \n"
			"class Foo \n"
			"{ \n"
			"	int m_i; \n"
			"	Bar@ m_bar;  \n"// comment this out to make the problem disappear

			"	Foo(int i) { m_i = i; } \n"
			"	~Foo() { print('dtor ' + m_i + '\\n'); } \n"
			"} \n"

			"bool SortFunc(const Foo@const & in a, const Foo@const & in b) \n"
			"{ \n"
			"	return b.m_i % 7 == 0; \n"
			"} \n"

			"array<Foo@> g_arr; \n"

			"void main() \n"
			"{ \n"
			"	print('start of main\\n'); \n"

			"	for (int i = 0; i < 500; i++) { \n"
			"		g_arr.insertLast(Foo(i)); \n"
			"	} \n"

			"	g_arr.sort(SortFunc); \n"

			"	print('end of main\\n'); \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		// No instances of Foo should have been destroyed so far
		if (g_printBuf != "start of main\n"
						  "end of main\n")
		{
			PRINTF("%s", g_printBuf.c_str());
			TEST_FAILED;
		}

		asUINT currSize, totDestr, totDetect;
		engine->GetGCStatistics(&currSize, &totDestr, &totDetect);
		if (currSize != 501)
			TEST_FAILED;
		if (totDestr != 0)
			TEST_FAILED;
		if (totDetect != 0)
			TEST_FAILED;

		mod->Discard();

		engine->GarbageCollect();

		engine->GetGCStatistics(&currSize, &totDestr, &totDetect);
		if (currSize != 0)
			TEST_FAILED;
		if (totDestr != 501)
			TEST_FAILED;
		if (totDetect != 0)
			TEST_FAILED;

		engine->ShutDownAndRelease();
	}

	// Test registering template specialization after registering the standard array add-on
	// https://www.gamedev.net/forums/topic/709563-dictionary-addon-doesnt-work-with-uint-values/5437222/
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		RegisterScriptArray(engine, false);

		r = engine->RegisterObjectType("array<int>", 0, asOBJ_REF);
		if (r < 0)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test array init list with object handles
	// https://www.gamedev.net/forums/topic/707320-bug-cscriptarray-last-item-is-null-for-arrayltobjectgt-initialized-by-list-initalizer-syntax/
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);
		RegisterScriptArray(engine, true);

		asIScriptModule* mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"class Object { Object(Object @o = null) {} } \n"
			"class List { \n"
			"	array<Object@> @objects = { \n"
			"	  @Object(), \n"
			"	  @Object(), \n"
			"	  @Object() \n"
			"	}; \n"
			"} \n"
			"void main() { \n"
			"  List@ list = List(); \n"
			"  assert( list.objects.length() == 3 ); \n"
			"  assert( list.objects[0] !is null ); \n"
			"  assert( list.objects[1] !is null ); \n"
			"  assert( list.objects[2] !is null ); \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test value assign from array holding handles
	// reported by Aaron Baker
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);
		RegisterScriptArray(engine, true);

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"class character {} \n"
			"void main() { \n"
			"  array<character@> party; \n"
			"  character copy; \n"
			"  copy = party[0]; \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		bout.buffer = "";
		mod->AddScriptSection("test",
			"class character { \n"
			"  character &opAssign(const character &) { return this; } \n"
			"} \n"
			"void main() { \n"
			"  array<character@> party; \n"
			"  character copy; \n"
			"  copy = party[0]; \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}
		
		engine->ShutDownAndRelease();
	}
	
	// Test initialization of array as default arg
	// https://www.gamedev.net/forums/topic/699878-array-argument-getting-wrong-default-value/
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);
		RegisterScriptArray(engine, false);

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"void f(array<int> a = {9}) { \n"
			"  assert(a[0] == 9); \n"
			"}﻿ \n"
			"void main() { f(); } \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;
		
		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}
	
	// Test circular reference between array and ref
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);
		RegisterScriptHandle(engine);
		RegisterScriptArray(engine, false);

		// Create the circular reference
		r = ExecuteString(engine, "array<ref> a; a.resize(1); @a[0] = a;");
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		engine->GarbageCollect();

		asUINT currSize, totDestroy, totDetect;
		engine->GetGCStatistics(&currSize, &totDestroy, &totDetect);
		if (currSize != 0 || totDestroy != 1 || totDetect != 1)
			TEST_FAILED;

		engine->ShutDownAndRelease();
	}

	// Test sort through callback
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);
		RegisterScriptArray(engine, false);

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"void main() { \n"
			"  array<int> a = {1,2,3,4,5,6}; \n"
			"  a.sort(function(a,b) { if( (a & 1) == (b & 1) ) return a < b; else return (a & 1) < (b & 1); }); \n"
			"  assert( a == {2,4,6,1,3,5} ); \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;
		
		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		CBytecodeStream bcStream("test");
		r = mod->SaveByteCode(&bcStream);
		if (r < 0)
			TEST_FAILED;

		mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		r = mod->LoadByteCode(&bcStream);
		if (r < 0)
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test sort through callback on array with const handles
	// https://www.gamedev.net/forums/topic/699740-how-to-pass-global-function-as-callback/
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);
		RegisterScriptArray(engine, false);

		engine->RegisterGlobalFunction("void print(const?&in)", asFUNCTION(print), asCALL_GENERIC);
		printResult.str("");

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"class Test { \n"
			"	int x; \n"
			"	Test() { x = 0; } \n"
			"	Test(int x1) { x = x1; } \n"
			"	int value { get const { return x + 10; } } \n"
			"} \n"
			"bool less(const Test @const &in a, const Test @const &in b) { //print(a); \n"
			"		return a.value<b.value; } \n"
			"void main() { \n"
			"	array<const Test@> a = { \n"
			"		Test(8), \n"
			"		Test(4), \n"
			"		Test(5), \n"
			"		Test(), \n"
			"		Test(7), \n"
			"		Test(1) \n"
			"	}; \n"
			"	print(a[0]); \n"
			"	print(Test(9)); \n"
			"	a.sort(less); \n"
			"	print(a[0]); \n"
			"	print(a[1]); \n"
			"	print(a[2]); \n"
			"	print(a[3]); \n"
			"	print(a[4]); \n"
			"	print(a[5]); \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		if (printResult.str() != "18\n19\n10\n11\n14\n15\n17\n18\n")
		{
			PRINTF("Wrong result, got: \n");
			PRINTF("%s", printResult.str().c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}


	// Test sort through callback with objects
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);
		RegisterScriptArray(engine, false);

		engine->RegisterGlobalFunction("void print(const?&in)", asFUNCTION(print), asCALL_GENERIC);
		printResult.str("");

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"class Test { \n"
			"	int x; \n"
			"	Test() { x = 0; } \n"
			"	Test(int x1) { x = x1; } \n"
			"	int value { get const { return x + 10; } } \n"
			"} \n"
			"void main() { \n"
			"	array<Test@> a = { \n"
			"		Test(8), \n"
			"		Test(4), \n"
			"		Test(5), \n"
			"		Test(), \n"
			"		Test(7), \n"
			"		Test(1) \n"
			"	}; \n"
			"	print(a[0]); \n"
			"	print(Test(9)); \n"
			"	a.sort(function(a, b) { \n"
			"		//print(a); \n"
			"		return a.value<b.value; \n"
			"	}); \n"
			"	print(a[0]); \n"
			"	print(a[1]); \n"
			"	print(a[2]); \n"
			"	print(a[3]); \n"
			"	print(a[4]); \n"
			"	print(a[5]); \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		if (printResult.str() != "18\n19\n10\n11\n14\n15\n17\n18\n")
		{
			PRINTF("Wrong result, got: \n");
			PRINTF("%s", printResult.str().c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test anonymous init lists
	{
		engine = asCreateScriptEngine();
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);
		RegisterScriptArray(engine, false);
		RegisterStdString(engine);
		RegisterScriptDictionary(engine);

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"array<int> a; \n"
			"void func(array<int> @a) { assert( a.length() == 3 ); } \n"
			"void main() \n"
			"{ \n"
			"  a = {1,2,3}; \n" // same as 'a = array<int> = {1,2,3};'
			"  assert( a.length() == 3 ); \n"
			"  func({1,2,3}); \n" // same as 'func(array<int> = {1,2,3});'
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;

		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		r = ExecuteString(engine, "main()", mod);
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;

		// Make sure the error message is clear when there is no matching function
		// Make sure the error message is clear when there are multiple matching functions
		// Test providing an init list for a different type
		bout.buffer = "";
		mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"void func(int a) { } \n"
			"void func2(array<int> @a) { } \n"
			"void func2(dictionary @d) { } \n"
			"void func3(dictionary @d) { } \n"
			"void main() \n"
			"{ \n"
			"  func({1,2,3}); \n"
			"  func2({1,2,3}); \n"
			"  func3({1,2,3}); \n"
			"} \n");
		r = mod->Build();
		if (r >= 0)
			TEST_FAILED;

		if (bout.buffer != "test (5, 1) : Info    : Compiling void main()\n"
						   "test (7, 3) : Error   : No matching signatures to 'func({...})'\n"
						   "test (7, 3) : Info    : Candidates are:\n"
						   "test (7, 3) : Info    : void func(int a)\n"
						   "test (8, 3) : Error   : Multiple matching signatures to 'func2({...})'\n"
						   "test (8, 3) : Info    : void func2(array<int>@ a)\n"
						   "test (8, 3) : Info    : void func2(dictionary@ d)\n"
						   "test (9, 10) : Error   : Expected a list enclosed by { } to match pattern\n"
						   "test (9, 9) : Error   : Previous error occurred while attempting to compile initialization list for type 'dictionary@'\n")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test array with object handles and insertLast
	// https://www.gamedev.net/topic/686912-array-with-objects-handles/
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);
		RegisterScriptArray(engine, false);

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"interface IFoo {} \n"
			"class CFoo : IFoo {} \n"
			"array<IFoo@> arr; \n"
			"void func(IFoo &f) { arr.insertLast(f); } \n"
			"void main() \n"
			"{ \n"
			"	CFoo foo(); \n"
			"	func(foo); \n"
			"	assert( arr.length() == 1 ); \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;
		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		ctx = engine->CreateContext();
		ctx->Prepare(mod->GetFunctionByName("main"));
		r = ctx->Execute();
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;
		ctx->Release();

		asIScriptFunction *func = mod->GetFunctionByName("func");
		asBYTE expect[] =
		{
			asBC_SUSPEND,
			asBC_PshVPtr,
			asBC_RefCpyV, // The refcpy here is required because the function expects a ref to a handle
			asBC_PopPtr,
			asBC_VAR,     // TODO: optimize: The local temporary handle is safe since it cannot be modified by anyone
			asBC_PshGPtr,
			asBC_CHKREF,
			asBC_GETREF,
			asBC_CALLSYS,
			asBC_FREE,
			asBC_SUSPEND,
			asBC_RET
		};
		if (!ValidateByteCode(func, expect))
			TEST_FAILED;

		engine->ShutDownAndRelease();
	}

	// Test array with weakref
	// http://www.gamedev.net/topic/680788-crash-when-trying-to-store-weakref-in-an-array/
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		RegisterScriptArray(engine, false);
		RegisterScriptWeakRef(engine);

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"class foo \n"
			"{ \n"
			"} \n"
			"int main() \n"
			"{ \n"
			"	array<weakref<foo>> arr; \n"
			"	foo f(); \n"
			"	arr.insertLast(weakref<foo>(f)); \n"
			"	return 1; \n"
			"} \n");
		r = mod->Build();
		if (r < 0)
			TEST_FAILED;
		if (bout.buffer != "")
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		ctx = engine->CreateContext();
		ctx->Prepare(mod->GetFunctionByName("main"));
		r = ctx->Execute();
		if (r != asEXECUTION_FINISHED)
			TEST_FAILED;
		ctx->Release();

		engine->ShutDownAndRelease();
	}

	// Test invalid initialization list
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		RegisterStdString(engine);
		RegisterScriptArray(engine, true);

		r = ExecuteString(engine, "int[] arr = {1,2,null};");
		if( r >= 0 )
			TEST_FAILED;

		r = ExecuteString(engine, "string[] arr = {'1','2',null};");
		if( r >= 0 )
			TEST_FAILED;

		if( bout.buffer != "ExecuteString (1, 18) : Error   : Can't implicitly convert from '<null handle>' to 'int&'.\n"
						   "ExecuteString (1, 25) : Error   : Can't implicitly convert from '<null handle>' to 'string&'.\n" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Test alternate syntax where empty list members give error
	// http://www.gamedev.net/topic/661578-array-trailing-comma/
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";

		engine->SetEngineProperty(asEP_DISALLOW_EMPTY_LIST_ELEMENTS, 1);

		RegisterScriptArray(engine, false);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		// The last comma should be ignored
		r = ExecuteString(engine, "array<int> arr = {1,2,3,}; assert( arr.length() == 3 );");
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		r = ExecuteString(engine, "array<int> arr = {,2,3,,5};");
		if( r >= 0 )
			TEST_FAILED;

		if( bout.buffer != "ExecuteString (1, 19) : Error   : Empty list element is not allowed\n"
						   "ExecuteString (1, 24) : Error   : Empty list element is not allowed\n" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->Release();
	}

	// Test releasing the script engine while a script array is still alive
	// It must be gracefully handled, preferrably with an appropriate error message
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);

		RegisterScriptArray(engine, false);

		CScriptArray *arr = CScriptArray::Create(engine->GetTypeInfoByDecl("array<int>"));
		
		engine->Release();

		arr->Release();
	}


	// Test empty initialization list
	// http://www.gamedev.net/topic/658849-empty-array-initialization/
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);

		RegisterScriptArray(engine, false);

		r = ExecuteString(engine, "array<int> a = {};");
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// Test exception in constructor of value type
	SKIP_ON_MAX_PORT
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);

		RegisterScriptArray(engine, false);

		engine->RegisterObjectType("Except", sizeof(ClassExceptionInConstructor), asOBJ_VALUE | asOBJ_APP_CLASS_CDA);
		engine->RegisterObjectBehaviour("Except", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ClassExceptionInConstructor::Construct), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectBehaviour("Except", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ClassExceptionInConstructor::Destruct), asCALL_CDECL_OBJLAST);
		engine->RegisterObjectMethod("Except", "Except &opAssign(const Except &in)", asMETHOD(ClassExceptionInConstructor, operator=), asCALL_THISCALL);

		ctx = engine->CreateContext();
		r = ExecuteString(engine, "array<Except> arr(2);", 0, ctx);
		if( r != asEXECUTION_EXCEPTION )
			TEST_FAILED;
		else if( std::string(ctx->GetExceptionString()) != "Caught an exception from the application" )
		{
			PRINTF("Got exception : %s\n", ctx->GetExceptionString());
			TEST_FAILED;
		}
		ctx->Release();

		engine->Release();
	}

	// Test initializing an array with string registered as reference
	// type, and skipping values in initialization list
	// http://www.gamedev.net/topic/653233-array-with-object-reference/
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream,Callback), &out, asCALL_THISCALL);
		RegisterScriptString(engine); // ref type
		RegisterScriptArray(engine, true);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"void main() { \n"
			"  string@[] a = {'a', , 'c', , 'e'}; assert( a[1] is null ); assert( a[2] == 'c' ); \n"
			"  string[] b = {'a', , 'c', , 'e'}; assert( b[1] == '' ); assert( b[2] == 'c' ); \n"
			"  int[] c = {1, , 3, , 5}; assert( c[1] == 0 ); assert( c[4] == 5 ); \n"
			"  const uint8[] d = {1,2,3,4,5}; assert( d[4] == 5 ); \n"
			"} \n");
		r = mod->Build();
		if( r < 0 )
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		CBytecodeStream stream("TestArray");
		r = mod->SaveByteCode(&stream);
		if( r < 0 )
			TEST_FAILED;

		mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		r = mod->LoadByteCode(&stream);
		if( r < 0 )
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// Compile array with default value in list
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream,Callback), &out, asCALL_THISCALL);
		RegisterStdString(engine); // value type
		RegisterScriptArray(engine, false);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		r = ExecuteString(engine, "array<string> a = {'a', , 'c', , 'e'}; assert( a[1] == '' );");
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// Compile nested array init list
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream,Callback), &out, asCALL_THISCALL);
		RegisterScriptArray(engine, false);

		r = ExecuteString(engine, "array<array<int>> a = {{1,2},{3,4}};");
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	
		engine->SetMessageCallback(asMETHOD(COutStream,Callback), &out, asCALL_THISCALL);

		RegisterScriptString(engine);
		RegisterScriptArray(engine, false);

		engine->RegisterGlobalFunction("void Assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);

		// Test sorting on array of handles
		mod->AddScriptSection(TESTNAME, 
			"class Test { \n"
			"  Test(int v) {val = v;} \n"
			"  int opCmp(const Test & o) const { return val - o.val; } \n"
			"  int val; \n"
			"} \n");
		r = mod->Build();
		if( r < 0 )
			TEST_FAILED;
		ctx = engine->CreateContext();
		r = ExecuteString(engine, 
			"array<Test @> a = { Test(1), Test(4), Test(2), null, Test(3) }; \n"
			"a.sortAsc(); \n"
			"Assert( a[0] is null ); \n"
			"Assert( a[1].val == 1 ); \n"
			"Assert( a[2].val == 2 ); \n"
			"Assert( a[3].val == 3 ); \n"
			"Assert( a[4].val == 4 ); \n", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;
		if( r == asEXECUTION_EXCEPTION )
			PRINTF("%s", GetExceptionInfo(ctx).c_str());
		ctx->Release();

		// Garbage collect the objects so the module can be reused
		engine->GarbageCollect();

		// Test different signatures on opCmp and opEquals
		mod->AddScriptSection(TESTNAME,
			"class C \n"
			"{ \n"
			"    C(int i) {i_ = i;} \n"
	//		"    bool opEquals (const C &in other) const\n"
	//		"    { \n"
	//		"        return i_ == other.i_; \n"
	//		"    } \n"
	//		"    int opCmp (const C &in other) const\n"
	//		"    { \n"
	//		"        return i_ - other.i_; \n"
	//		"    } \n"
			"    bool opEquals (const C @ other) const\n"
			"    { \n"
			"        return i_ == other.i_; \n"
			"    } \n"
			"    int opCmp (const C @ other) const\n"
			"    { \n"
			"        return i_ - other.i_; \n"
			"    } \n"
			"    int i_; \n"
			"} \n"
			"void main (void) \n"
			"{ \n"
			"    array<const C @> a2; \n"
			"    a2.insertLast(@C(2)); \n"
			"    a2.insertLast(@C(1)); \n"
			"    a2.sortAsc(); \n"
			"    Assert( a2[0].i_ == 1 ); \n"
			"    Assert( a2[1].i_ == 2 ); \n"
			"    C f(2); \n"
			"    Assert( a2.find(f) == 1 ); \n"
			"} \n");
		r = mod->Build();
		if( r < 0 )
			TEST_FAILED;
		ctx = engine->CreateContext();
		r = ExecuteString(engine, "main()", mod, ctx);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;
		if( r == asEXECUTION_EXCEPTION )
			PRINTF("%s", GetExceptionInfo(ctx).c_str());
		ctx->Release();

		// Garbage collect the objects so the module can be reused
		engine->GarbageCollect();

		// Multiple tests in one
		mod->AddScriptSection(TESTNAME, script1, strlen(script1), 0);
		r = mod->Build();
		if( r < 0 )
		{
			TEST_FAILED;
			PRINTF("%s: Failed to compile the script\n", TESTNAME);
		}

		ctx = engine->CreateContext();
		r = ExecuteString(engine, "TestArray()", mod, ctx);
		if( r != asEXECUTION_FINISHED )
		{
			if( r == asEXECUTION_EXCEPTION )
				PRINTF("%s", GetExceptionInfo(ctx).c_str());

			PRINTF("%s: Failed to execute script\n", TESTNAME);
			TEST_FAILED;
		}
		if( ctx ) ctx->Release();

		mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection(TESTNAME, script2, strlen(script2), 0);
		r = mod->Build();
		if( r < 0 )
		{
			TEST_FAILED;
			PRINTF("%s: Failed to compile the script\n", TESTNAME);
		}

		r = ExecuteString(engine, "TestArrayException()", mod);
		if( r != asEXECUTION_EXCEPTION )
		{
			PRINTF("%s: No exception\n", TESTNAME);
			TEST_FAILED;
		}

		// Must be possible to declare array of arrays
		mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection(TESTNAME, script3, strlen(script3), 0);
		r = mod->Build();
		if( r < 0 )
		{
			TEST_FAILED;
			PRINTF("%s: Failed to compile the script\n", TESTNAME);
		}

		ctx = engine->CreateContext();
		r = ExecuteString(engine, "TestArrayMulti()", mod, ctx);
		if( r != asEXECUTION_FINISHED )
		{
			PRINTF("%s: Failure\n", TESTNAME);
			TEST_FAILED;
		}
		if( r == asEXECUTION_EXCEPTION )
		{
			PRINTF("%s", GetExceptionInfo(ctx).c_str());
		}
		if( ctx ) ctx->Release();
		ctx = 0;


		mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection(TESTNAME, script4, strlen(script4), 0);
		r = mod->Build();
		if( r < 0 )
		{
			TEST_FAILED;
			PRINTF("%s: Failed to compile the script\n", TESTNAME);
		}
		ctx = engine->CreateContext();
		r = ExecuteString(engine, "TestArrayChar()", mod, ctx);
		if( r != asEXECUTION_FINISHED )
		{
			PRINTF("%s: Failure\n", TESTNAME);
			TEST_FAILED;
		}
		if( r == asEXECUTION_EXCEPTION )
		{
			PRINTF("%s", GetExceptionInfo(ctx).c_str());
		}

		if( ctx ) ctx->Release();

		// Initialization lists must work for array template
		mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection(TESTNAME, script5, strlen(script5), 0);
		r = mod->Build();
		if( r < 0 ) TEST_FAILED;
		ctx = engine->CreateContext();
		r = ExecuteString(engine, "TestArrayInitList()", mod, ctx);
		if( r != asEXECUTION_FINISHED ) TEST_FAILED;
		if( r == asEXECUTION_EXCEPTION )
			PRINTF("%s", GetExceptionInfo(ctx).c_str());

		if( ctx ) ctx->Release();

		engine->SetMessageCallback(asMETHOD(CBufferedOutStream,Callback), &bout, asCALL_THISCALL);
		bout.buffer = "";
		mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection(TESTNAME, script6, strlen(script6), 0);
		r = mod->Build();
		if( r >= 0 ) TEST_FAILED;
		if( bout.buffer != "Test_Addon_ScriptArray (1, 1) : Info    : Compiling void Test()\n"
						   "Test_Addon_ScriptArray (4, 21) : Error   : Initialization lists cannot be used with 'int'\n" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		// Array object must call default constructor of the script classes
		engine->SetMessageCallback(asMETHOD(COutStream,Callback), &out, asCALL_THISCALL);
		mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection(TESTNAME, script7, strlen(script7), 0);
		r = mod->Build();
		if( r < 0 ) 
			TEST_FAILED;
		r = ExecuteString(engine, "Test()", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;
		
		// Test bool[] on Mac OS X with PPC CPU
		// Submitted by Edward Rudd
		const char *script8 =
		"array<bool> f(10);         \n"
		"for (int i=0; i<10; i++) { \n"
		"	f[i] = false;           \n"
		"}                          \n"
		"Assert(f[0] == false);     \n"
		"Assert(f[1] == false);     \n"
		"f[0] = true;               \n"
		"Assert(f[0] == true);      \n"
		"Assert(f[1] == false);     \n";
	
		r = ExecuteString(engine, script8, mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		// Test reserve()
		{
			const char *script = 
				"array<int> f; \n"
				"f.reserve(10); \n"
				"for( uint n = 0; n < 10; n++ ) \n"
				"  f.insertAt(n, n); \n"
				"Assert( f.length() == 10 ); \n";
			r = ExecuteString(engine, script, mod);
			if( r != asEXECUTION_FINISHED )
				TEST_FAILED;
		}

		// Make sure it is possible to do multiple assignments with the array type
		r = ExecuteString(engine, "array<int> a, b, c; a = b = c;");
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		// Must support syntax as: array<array<int>>, i.e. without white space between the closing angled brackets.
		r = ExecuteString(engine, "array<array<int>> a(2); Assert( a.length() == 2 );");
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		// Must support arrays of handles
		r = ExecuteString(engine, "array<array<int>@> a(1); @a[0] = @array<int>(4);");
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		// resize initializes the members to zero
		r = ExecuteString(engine, "array<int> a; a.resize(2); Assert( a[1] == 0 ); \n");
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		// Do not allow the instantiation of a template with a subtype that cannot be created
		bout.buffer = "";
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		engine->RegisterObjectType("single", 0, asOBJ_REF | asOBJ_NOHANDLE);
		r = ExecuteString(engine, "array<single> a;");
		if( r >= 0 )
			TEST_FAILED;
		if( bout.buffer != "array (0, 0) : Error   : The subtype 'single' has no default factory\n"
						   "ExecuteString (1, 7) : Error   : Attempting to instantiate invalid template type 'array<single>'\n" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->Release();
	}

	// Test too large arrays
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		RegisterScriptArray(engine, false);

		ctx = engine->CreateContext();
		r = ExecuteString(engine, "array<int> a; a.resize(0xFFFFFFFF);", 0, ctx);
		if( r != asEXECUTION_EXCEPTION )
			TEST_FAILED;
		else if( strcmp(ctx->GetExceptionString(), "Too large array size") != 0 )
			TEST_FAILED;

		r = ExecuteString(engine, "array<int> a(0xFFFFFFFF);", 0, ctx);
		if( r != asEXECUTION_EXCEPTION )
			TEST_FAILED;
		else if( strcmp(ctx->GetExceptionString(), "Too large array size") != 0 )
			TEST_FAILED;

		r = ExecuteString(engine, "array<int> list;\n"
								  "list.resize(3);\n"
								  "list.reserve(-1);\n", 0, ctx);
		if( r != asEXECUTION_EXCEPTION )
			TEST_FAILED;
		else if( strcmp(ctx->GetExceptionString(), "Too large array size") != 0 )
			TEST_FAILED;

		ctx->Release();
		engine->Release();
	}

	// Test garbage collect with script class that holds array member
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		RegisterScriptArray(engine, false);

		asIScriptModule *mod = engine->GetModule("module", asGM_ALWAYS_CREATE);

		const char *script = 
			"class MyTest \n"
			"{ \n"
			"	array<int> myList; \n"
			"} \n";

		mod->AddScriptSection("script", script);
		r = mod->Build();
		if( r < 0 )
			TEST_FAILED;

		asIScriptObject *obj = (asIScriptObject*)engine->CreateScriptObject(mod->GetTypeInfoByName("MyTest"));
		obj->Release();

		engine->Release();
	}

	// Test the default value constructor
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);
		RegisterScriptArray(engine, false);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		asIScriptModule *mod = engine->GetModule("module", asGM_ALWAYS_CREATE);
		engine->SetEngineProperty(asEP_OPTIMIZE_BYTECODE, false);

		const char *script = 
			"void main() \n"
			"{ \n"
			"	array<int> arr(2, 42); \n"
			"   assert(arr[0] == 42); \n"
			"   assert(arr[1] == 42); \n"
			"   array<array<int>> arr2(2, array<int>(2)); \n"
			"   assert(arr2[0].length() == 2); \n"
			"	assert(arr2[1].length() == 2); \n"
			"   array<array<int>@> arr3(2, arr); \n"
			"   assert(arr3[0] is arr); \n"
			"   assert(arr3[1] is arr); \n"
			"} \n";

		mod->AddScriptSection("script", script);
		r = mod->Build();
		if( r < 0 )
			TEST_FAILED;

		r = ExecuteString(engine, "main()", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// Test potential memory leak situation with circular reference between types
	{
		// Create the script engine
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	 
		// Register array class
		RegisterScriptArray(engine, false);
	 
		// Compile
		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", 
			"class Hoge"
			"{"
			"    HogeManager@ hogeManager;"
			"};"
			"class HogeManager"
			"{"
			"    array< Hoge >@ hoges;"
			"};"
			, 0);
		mod->Build();
	 
		// Release engine
		engine->Release();
	}

	// Test creating script array from application
	{
		SKIP_ON_MAX_PORT
		{
			engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
			RegisterScriptArray(engine, false);
			RegisterScriptString(engine);
		
			r = engine->RegisterGlobalFunction("array<string@>@ CreateArrayOfStrings()", asFUNCTION(CreateArrayOfStrings), asCALL_CDECL); assert( r >= 0 );

			r = ExecuteString(engine, "array<string@>@ arr = CreateArrayOfStrings()");
			if( r != asEXECUTION_FINISHED )
				TEST_FAILED;
		 
			// Release engine
			engine->Release();
		}		
	}

	// Test insertAt, removeAt, removeRange
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);
		RegisterScriptArray(engine, false);
		RegisterScriptString(engine);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		r = ExecuteString(engine,
			"array<string> arr = {'1','2','3'}; \n"
			"arr.insertAt(1, 'test'); \n"
			"assert( arr[1] == 'test' );\n"
			"arr.insertAt(4, '4'); \n"
			"assert( arr[4] == '4' );\n"
			"arr.removeAt(0); \n"
			"assert( arr[0] == 'test' );\n"
			"assert( arr[3] == '4' );\n"
			"arr.removeRange(1, 2); \n"
			"assert( arr == (array<string> = {'test', '4'}) ); \n"
			"arr.insertAt(1, array<string> = {'1','2','3'}); \n"
			"assert( arr == (array<string> = {'test', '1','2','3','4'}) ); \n"
			// insertAt on itself
			"arr = (array<string> = {'1','2'}); \n"
			"arr.insertAt(1, arr); \n"
			"assert( arr == (array<string> = {'1','1','2','2'}) ); \n");
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// This test was failing on XBOX 360
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);

		RegisterScriptArray(engine, true);
		
		const char *script = 
			"class ArrayOf  \n"
			"{  \n"
			"  uint8[] _boolList;  \n"
			"  int _numOfStockedObject;  \n"
			"  ArrayOf(int arraySizeMax)  \n"
			"  {  \n"
			"    _boolList.resize(arraySizeMax);  \n"
			"    _numOfStockedObject = 0;  \n"
			"    for(int i = 0; i < arraySizeMax; ++i)  \n"
			"    {  \n"
			"       _boolList[i] = 0; \n"
			"    } \n"
			"  } \n"
			"} \n";

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		r = mod->Build();
		if( r < 0 ) TEST_FAILED;
		r = ExecuteString(engine, "ArrayOf(100)", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// Array should call subtypes' opAssign when it exists
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);
		RegisterScriptArray(engine, true);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		const char *script = 
			"int calls = 0; \n"
			"class Value  \n"
			"{  \n"
			"  int val;  \n"
			"  Value(int v) {val = v;} \n"
			"  Value() {} \n"
			"  Value &opAssign(const Value &in o) { calls++; return this; } \n"
			"} \n";

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		r = mod->Build();
		if( r < 0 ) TEST_FAILED;
		r = ExecuteString(engine, "array<Value> arr = {Value(2), Value(3), Value(0)}; \n"
								  "assert( calls == 0 ); \n"
								  "array<Value> arr2; \n"
								  "arr2 = arr; \n"
								  "assert( calls == 3 ); \n", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// test sorting
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);
		RegisterScriptArray(engine, true);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		const char *script = 
			"class Value  \n"
			"{  \n"
			"  int val;  \n"
			"  Value(int v) {val = v;} \n"
			"  Value() {} \n"
			"  int opCmp(const Value &in o) {return val - o.val;} \n"
			"} \n";

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		r = mod->Build();
		if( r < 0 ) TEST_FAILED;
		r = ExecuteString(engine, "Value[] arr = {Value(2), Value(3), Value(0)}; \n"
			                      "arr.sortAsc(); \n"
								  "assert(arr[0].val == 0); \n"
								  "assert(arr[1].val == 2); \n"
								  "assert(arr[2].val == 3);", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// Test 
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);
		RegisterScriptArray(engine, true);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		const char *script = 
			"bool TestSort() \n"
			"{ \n"
			"	array<int> A = {1, 5, 2, 4, 3}; \n"
			"	array<int> B = {1, 5, 2, 4, 3}; \n"
			"	A.sortAsc(); \n"
			"	B.sortDesc(); \n"
			"	return \n"
			"		A[0] == 1 && A[1] == 2 && A[2] == 3 && A[3] == 4 && A[4] == 5 && \n"
			"		B[0] == 5 && B[1] == 4 && B[2] == 3 && B[3] == 2 && B[4] == 1; \n"
			"} \n"
			"bool TestReverse() \n"
			"{ \n"
			"	array<int> A = {5, 4, 3, 2, 1}; \n"
			"	A.reverse(); \n"
			"	return A[0] == 1 && A[1] == 2 && A[2] == 3 && A[3] == 4 && A[4] == 5; \n"
			"} \n"
			"class cOpCmp \n"
			"{ \n"
			"	cOpCmp() \n"
			"	{ \n"
			"		a = 0; \n"
			"		b = 0.0; \n"
			"	}	 \n"
			"	cOpCmp(int _a, float _b) \n"
			"	{ \n"
			"		a = _a; \n"
			"		b = _b;	 \n"
			"	} \n"
			"	void set(int _a, float _b) \n"
			"	{ \n"
			"		a = _a; \n"
			"		b = _b; \n"
			"	} \n"
			"	int opCmp(cOpCmp &in other) \n"
			"	{ \n"
			"		return a - other.a; \n"
			"	} \n"
			"	int a; \n"
			"	float b; \n"
			"} \n"
			"class cOpEquals \n"
			"{ \n"
			"	cOpEquals() \n"
			"	{ \n"
			"		a = 0; \n"
			"		b = 0.0; \n"
			"	}	 \n"
			"	cOpEquals(int _a, float _b) \n"
			"	{ \n"
			"		a = _a; \n"
			"		b = _b;	 \n"
			"	} \n"
			"	void set(int _a, float _b) \n"
			"	{ \n"
			"		a = _a; \n"
			"		b = _b; \n"
			"	} \n"
			"	bool opEquals(cOpEquals &in other) \n"
			"	{ \n"
			"		return a == other.a; \n"
			"	} \n"
			"	int a; \n"
			"	float b; \n"
			"} \n"
			"bool TestFind() \n"
			"{ \n"
			"	array<int> A = {5, 8, 3, 2, 0, 0, 2, 1}; \n"
			"	if (A.find(10) != -1) \n"
			"		return false; \n"
			"	if (A.find(0) != 4) \n"
			"		return false; \n"
			"	if (A.find(1, 8) != 1) \n"
			"		return false; \n"
			"	if (A.find(2, 8) != -1) \n"
			"		return false; \n"
			"	array<cOpCmp> CMP(5); \n"
			"	CMP[0].set(0, 0.0); \n"
			"	CMP[1].set(1, 0.0); \n"
			"	CMP[2].set(2, 0.0); \n"
			"	CMP[3].set(3, 0.0); \n"
			"	CMP[4].set(4, 0.0);	\n"
			"	if (CMP.find(cOpCmp(5, 0.0)) != -1) \n"
			"		return false; \n"
			"	if (CMP.find(2, cOpCmp(2, 1.0)) != 2) \n"
			"		return false; \n"
			"	if (CMP.find(3, cOpCmp(2, 1.0)) != -1) \n"
			"		return false; \n"
			"	array<cOpEquals> EQ(5); \n"
			"	EQ[0].set(0, 0.0); \n"
			"	EQ[1].set(1, 0.0); \n"
			"	EQ[2].set(2, 0.0); \n"
			"	EQ[3].set(3, 0.0); \n"
			"	EQ[4].set(4, 0.0); \n"
			"	if (EQ.find(cOpEquals(5, 0.0)) != -1) \n"
			"		return false; \n"
			"	if (EQ.find(2, cOpEquals(2, 1.0)) != 2) \n"
			"		return false; \n"
			"	if (EQ.find(3, cOpEquals(2, 1.0)) != -1) \n"
			"		return false; \n"
			"	return true; \n"
			"} \n"
			"int main() \n"
			"{ \n"
			"	assert( TestSort() ); \n"
			"	assert( TestReverse() ); \n"
			"	assert( TestFind() ); \n"
			"	return 789; \n"
			"} \n";

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		r = mod->Build();
		if( r < 0 ) TEST_FAILED;
		r = ExecuteString(engine, "assert( main() == 789 ); \n", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;

		engine->Release();
	}

	// Test array, with objects that don't have default constructor/factory
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		RegisterScriptArray(engine, true);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		const char *script = 
			"class CTest \n"
			"{ \n"
			"  CTest(int v) {} \n" // With an explicit non-default constructor the compiler won't create the default constructor
			"} \n"
			"array<CTest> arr; \n";

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		bout.buffer = "";
		r = mod->Build();
		if( r > 0 ) 
			TEST_FAILED;
		if( bout.buffer != "array (0, 0) : Error   : The subtype 'CTest' has no default factory\n"
						   "script (5, 7) : Error   : Attempting to instantiate invalid template type 'array<CTest>'\n" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->Release();
	}	

	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		RegisterScriptArray(engine, true);

		const char *script = 
			"class T { }; \n"
			"array<T> arr; \n";

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		bout.buffer = "";
		r = mod->Build();
		if( r < 0 ) 
			TEST_FAILED;
		if( bout.buffer != "" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}
		r = ExecuteString(engine, "array<T> arr(1); \n", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;
		
		engine->Release();
	}

	// Test problem with arrays of enums reported by Philip Bennefall
	{
		const char *script = 
			"enum fruit \n"
			"{ \n"
			"  APPLE, ORANGE, BANANA \n"
			"} \n"
			"void main() \n"
			"{ \n"
			"  fruit[] basket; \n"
			"  basket.insertLast(APPLE); \n"
			"  basket.insertLast(ORANGE); \n"
			"  basket.sortDesc(); \n"
			"  int index = basket.find(APPLE); \n"
			"  assert( index == 1 ); \n"
			"} \n";

		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		RegisterScriptArray(engine, true);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		bout.buffer = "";
		r = mod->Build();
		if( r < 0 ) 
			TEST_FAILED;
		if( bout.buffer != "" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}
		r = ExecuteString(engine, "main()", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;
		
		engine->Release();
	}

	// Test problem with arrays and opEquals reported by Philip Bennefall
	{
		const char *script = 
			"class fish \n"
			"{ \n"
			"  bool opEquals(fish@ other) \n" // handles should be supported too
			"  { \n"
			"    return false; \n"
			"  } \n"
			"} \n"
			"void main() \n"
			"{ \n"
			"  fish[] ocean(100); \n"
			"  fish nemo; \n"
			"  int index = ocean.find(nemo); \n"
			"} \n";

		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		RegisterScriptArray(engine, true);

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		bout.buffer = "";
		r = mod->Build();
		if( r < 0 ) 
			TEST_FAILED;
		if( bout.buffer != "" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}
		r = ExecuteString(engine, "main()", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;
		
		engine->Release();
	}

	// Test problem with arrays and opEquals reported by Philip Bennefall
	{
		const char *script = 
			"class fish \n"
			"{ \n"
			"  bool opEquals(fish@ other) \n"
			"  { \n"
			"    return false; \n"
			"  } \n"
			"} \n"
			"void main() \n"
			"{ \n"
			"  fish@[] ocean(100); \n"
			"  for(uint i=0; i<ocean.length(); i++) \n"
			"  { \n"
			"    fish fred; \n"
			"    @(ocean[i]) = fred; \n"
			"  } \n"
			"  fish nemo; \n"
			"  int index = ocean.find(nemo); \n"
			"} \n";

		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);
		RegisterScriptArray(engine, true);

		asIScriptModule *mod = engine->GetModule(0, asGM_ALWAYS_CREATE);
		mod->AddScriptSection("script", script);
		bout.buffer = "";
		r = mod->Build();
		if( r < 0 ) 
			TEST_FAILED;
		if( bout.buffer != "" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}
		r = ExecuteString(engine, "main()", mod);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;
		
		engine->Release();
	}

	// Test problem with arrays and opAssign reported by Philip Bennefall
	{
		const char *script = 
			"array<uint> a, b = {0,1,2,3}; \n"
			"a.reserve(10); \n"
			"a = b; \n"
			"assert( a.length() == b.length() ); \n"
			"assert( a.length() == 4 ); \n"
			"for( uint n = 0; n < a.length(); n++ ) \n"
			"  assert( a[n] == n ); \n";

		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);
		RegisterScriptArray(engine, true);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		r = ExecuteString(engine, script);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;
		
		engine->Release();
	}

	// Test findByRef
	{
		const char *script =
			"class Obj {} \n"
			"array<int> ia = {1,2,3}; \n"
			"array<Obj> oa = {Obj(), Obj()}; \n"
			"array<Obj@> ha = {Obj(), Obj()}; \n";

		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);
		RegisterScriptArray(engine, true);
		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);

		asIScriptModule *mod = engine->GetModule("mod", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test", script);
		r = mod->Build();
		if( r < 0 )
			TEST_FAILED;

		ctx = engine->CreateContext();
		r = ExecuteString(engine, "assert( ia.findByRef(ia[1]) == -1 ); \n"
								  "Obj @obj = oa[1]; \n"
								  "assert( oa.findByRef(obj) == 1 ); \n"
								  "@obj = ha[1]; \n"
								  "assert( ha.findByRef(obj) == 1 ); \n"
								  "ha.insertLast(null); \n"
								  "assert( ha.findByRef(null) == 2 ); \n", mod, ctx);
		if( r != asEXECUTION_FINISHED )
			TEST_FAILED;
		if( r == asEXECUTION_EXCEPTION )
			PRINTF("%s", GetExceptionInfo(ctx).c_str());

		ctx->Release();
		
		engine->Release();
	}

	// Test that array of handles to final class won't be garbage collected 
	{
		engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);

		RegisterScriptArray(engine, false);

		asIScriptModule *mod = engine->GetModule("test", asGM_ALWAYS_CREATE);
		mod->AddScriptSection("test",
			"class A {} \n"
			"final class B : A {} \n"
			"array<A@> arrA; \n"
			"array<B@> arrB; \n");
		r = mod->Build();
		if( r < 0 )
			TEST_FAILED;

		asITypeInfo *arrAType = mod->GetTypeInfoByDecl("array<A@>");
		asITypeInfo *arrBType = mod->GetTypeInfoByDecl("array<B@>");

		// array<A@> must be garbage collected since it is not possible to know that 
		// array can't hold a handle to a type derived from A that might cause circular 
		// reference with the array
		if( (arrAType->GetFlags() & asOBJ_GC) == 0 )
			TEST_FAILED;

		// array<B@> must not be garbage collected since it is known that the array
		// cannot form any circular reference with B.
		if( (arrBType->GetFlags() & asOBJ_GC) != 0 )
			TEST_FAILED;

		engine->Release();
	}

	// Test arrays with asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE
	// http://www.gamedev.net/topic/662655-array-of-arrays-with-asep-disallow-value-assign-for-ref-type-not-working/
	{
		engine = asCreateScriptEngine();
		engine->SetEngineProperty(asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE, true);
		engine->SetMessageCallback(asMETHOD(CBufferedOutStream, Callback), &bout, asCALL_THISCALL);

		engine->RegisterGlobalFunction("void assert(bool)", asFUNCTION(Assert), asCALL_GENERIC);
		RegisterScriptArray(engine, false);

		// array<array<float>> is not valid since it is not allowed to copy the subarray
		bout.buffer = "";
		r = ExecuteString(engine, "array<array<float>> a;");
		if( r >= 0 )
			TEST_FAILED;
		if( bout.buffer != "array (0, 0) : Error   : The subtype 'array<float>' has no default factory\n"
						   "ExecuteString (1, 7) : Error   : Attempting to instantiate invalid template type 'array<array<float>>'\n" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		// array<array<float>@> is valid, as the sub array is by handle
		bout.buffer = "";
		r = ExecuteString(engine, "array<array<float>@> a = { {1,2}, {2,3} }; \n"
								  "assert( a.length() == 2 ); \n"
								  "assert( a[0].length() == 2 ); \n"
								  "assert( a[1].length() == 2 ); \n"
								  "assert( a[1][1] == 3 ); \n");
		if( r < 0 )
			TEST_FAILED;
		if( bout.buffer != "" )
		{
			PRINTF("%s", bout.buffer.c_str());
			TEST_FAILED;
		}

		engine->ShutDownAndRelease();
	}

	// Success
	return fail;
}

bool Test2()
{
	bool fail = false;
	COutStream out;

	const char *script =
	"class A                               \n"
	"{                                     \n"
	"	int x;                             \n"
	"}                                     \n"
	"int sum(const array<A>& a)            \n"
	"{                                     \n"
	"	int s = 0;                         \n"
	"	for (uint i=0; i<a.length(); i++)  \n"
	"		s+=a[i].x;                     \n"
	"	return s;                          \n"
	"}                                     \n";

	const char *exec =
	"array<A> As;  \n"
    "As.resize(2); \n"
	"As[0].x = 1;  \n"
	"As[1].x = 2;  \n"
	"sum(As);      \n";

	asIScriptEngine *engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	engine->SetMessageCallback(asMETHOD(COutStream, Callback), &out, asCALL_THISCALL);
	RegisterScriptArray(engine, false);
	asIScriptModule *module = engine->GetModule("module", asGM_ALWAYS_CREATE);

	module->AddScriptSection("script", script);

	int r = module->Build();
	if( r < 0 )
	{
		TEST_FAILED;
	}

	r = ExecuteString(engine, exec, module);
	if( r != asEXECUTION_FINISHED )
	{
		TEST_FAILED;
	}

	engine->Release();

	return fail;
}

} // namespace

