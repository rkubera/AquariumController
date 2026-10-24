#pragma once
#if AC_ENABLE_PERIPHERALS && AC_ACTIVE_RELAYS==4 && AC_ACTIVE_PWM==6 && AC_ACTIVE_INPUTS==8 && AC_ENABLE_DHT && AC_ENABLE_RTC && AC_ENABLE_BUZZER && AC_ACTIVE_FAN
static const uint16_t AC_TELEMETRY_COUNT=AC_WIRE_FIELD_COUNT;
static uint16_t acTelemetryIndex(uint16_t offset){return offset<AC_TELEMETRY_COUNT?offset:UINT16_MAX;}
#else
static const uint16_t AC_ACTIVE_TELEMETRY_IDS[] PROGMEM={
#if 1
1,
#endif
#if 1
2,
#endif
#if 1
3,
#endif
#if 1
4,
#endif
#if 1
5,
#endif
#if 1
6,
#endif
#if 1
7,
#endif
#if AC_ACTIVE_DIAGNOSTICS
8,
#endif
#if AC_ACTIVE_DIAGNOSTICS
9,
#endif
#if AC_ACTIVE_DIAGNOSTICS
10,
#endif
#if AC_ENABLE_BUZZER
11,
#endif
#if AC_ENABLE_BUZZER
12,
#endif
#if AC_ENABLE_BUZZER
13,
#endif
#if AC_ENABLE_BUZZER
14,
#endif
#if AC_ENABLE_BUZZER && AC_ACTIVE_DIAGNOSTICS
15,
#endif
#if AC_ENABLE_BUZZER
16,
#endif
#if AC_ENABLE_RTC
17,
#endif
#if 1
18,
#endif
#if 1
19,
#endif
#if AC_ACTIVE_DIAGNOSTICS
20,
#endif
#if 1
21,
#endif
#if 1
22,
#endif
#if AC_ACTIVE_FAN
23,
#endif
#if AC_ACTIVE_FAN
24,
#endif
#if AC_ACTIVE_FAN
25,
#endif
#if 1
26,
#endif
#if AC_ENABLE_DHT
27,
#endif
#if AC_ENABLE_DHT
28,
#endif
#if 1
29,
#endif
#if 1
30,
#endif
#if 1
31,
#endif
#if 1
32,
#endif
#if 1
33,
#endif
#if 1
34,
#endif
#if 1
35,
#endif
#if AC_ENABLE_DHT
36,
#endif
#if 1
37,
#endif
#if 1
38,
#endif
#if 1
39,
#endif
#if 1
40,
#endif
#if 1
41,
#endif
#if 1
42,
#endif
#if AC_ACTIVE_PWM >= 1
43,
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
44,
#endif
#if AC_ACTIVE_PWM >= 1
45,
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ENABLE_PERIPHERALS
46,
#endif
#if AC_ACTIVE_PWM >= 1
47,
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
48,
#endif
#if AC_ACTIVE_PWM >= 1
49,
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
50,
#endif
#if AC_ACTIVE_PWM >= 1
51,
#endif
#if AC_ACTIVE_PWM >= 1
52,
#endif
#if AC_ACTIVE_PWM >= 1
53,
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
54,
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
55,
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
56,
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
57,
#endif
#if AC_ACTIVE_PWM >= 1 && AC_ACTIVE_INPUTS > 0
58,
#endif
#if AC_ACTIVE_PWM >= 1
59,
#endif
#if AC_ACTIVE_PWM >= 2
60,
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
61,
#endif
#if AC_ACTIVE_PWM >= 2
62,
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ENABLE_PERIPHERALS
63,
#endif
#if AC_ACTIVE_PWM >= 2
64,
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
65,
#endif
#if AC_ACTIVE_PWM >= 2
66,
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
67,
#endif
#if AC_ACTIVE_PWM >= 2
68,
#endif
#if AC_ACTIVE_PWM >= 2
69,
#endif
#if AC_ACTIVE_PWM >= 2
70,
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
71,
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
72,
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
73,
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
74,
#endif
#if AC_ACTIVE_PWM >= 2 && AC_ACTIVE_INPUTS > 0
75,
#endif
#if AC_ACTIVE_PWM >= 2
76,
#endif
#if AC_ACTIVE_PWM >= 3
77,
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
78,
#endif
#if AC_ACTIVE_PWM >= 3
79,
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ENABLE_PERIPHERALS
80,
#endif
#if AC_ACTIVE_PWM >= 3
81,
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
82,
#endif
#if AC_ACTIVE_PWM >= 3
83,
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
84,
#endif
#if AC_ACTIVE_PWM >= 3
85,
#endif
#if AC_ACTIVE_PWM >= 3
86,
#endif
#if AC_ACTIVE_PWM >= 3
87,
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
88,
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
89,
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
90,
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
91,
#endif
#if AC_ACTIVE_PWM >= 3 && AC_ACTIVE_INPUTS > 0
92,
#endif
#if AC_ACTIVE_PWM >= 3
93,
#endif
#if AC_ACTIVE_PWM >= 4
94,
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
95,
#endif
#if AC_ACTIVE_PWM >= 4
96,
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ENABLE_PERIPHERALS
97,
#endif
#if AC_ACTIVE_PWM >= 4
98,
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
99,
#endif
#if AC_ACTIVE_PWM >= 4
100,
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
101,
#endif
#if AC_ACTIVE_PWM >= 4
102,
#endif
#if AC_ACTIVE_PWM >= 4
103,
#endif
#if AC_ACTIVE_PWM >= 4
104,
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
105,
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
106,
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
107,
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
108,
#endif
#if AC_ACTIVE_PWM >= 4 && AC_ACTIVE_INPUTS > 0
109,
#endif
#if AC_ACTIVE_PWM >= 4
110,
#endif
#if AC_ACTIVE_PWM >= 5
111,
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
112,
#endif
#if AC_ACTIVE_PWM >= 5
113,
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ENABLE_PERIPHERALS
114,
#endif
#if AC_ACTIVE_PWM >= 5
115,
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
116,
#endif
#if AC_ACTIVE_PWM >= 5
117,
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
118,
#endif
#if AC_ACTIVE_PWM >= 5
119,
#endif
#if AC_ACTIVE_PWM >= 5
120,
#endif
#if AC_ACTIVE_PWM >= 5
121,
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
122,
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
123,
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
124,
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
125,
#endif
#if AC_ACTIVE_PWM >= 5 && AC_ACTIVE_INPUTS > 0
126,
#endif
#if AC_ACTIVE_PWM >= 5
127,
#endif
#if AC_ACTIVE_PWM >= 6
128,
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
129,
#endif
#if AC_ACTIVE_PWM >= 6
130,
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ENABLE_PERIPHERALS
131,
#endif
#if AC_ACTIVE_PWM >= 6
132,
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
133,
#endif
#if AC_ACTIVE_PWM >= 6
134,
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
135,
#endif
#if AC_ACTIVE_PWM >= 6
136,
#endif
#if AC_ACTIVE_PWM >= 6
137,
#endif
#if AC_ACTIVE_PWM >= 6
138,
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
139,
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
140,
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
141,
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
142,
#endif
#if AC_ACTIVE_PWM >= 6 && AC_ACTIVE_INPUTS > 0
143,
#endif
#if AC_ACTIVE_PWM >= 6
144,
#endif
#if AC_ACTIVE_RELAYS >= 1
145,
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
146,
#endif
#if AC_ACTIVE_RELAYS >= 1
147,
#endif
#if AC_ACTIVE_RELAYS >= 1
148,
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
149,
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
150,
#endif
#if AC_ACTIVE_RELAYS >= 1
151,
#endif
#if AC_ACTIVE_RELAYS >= 1
152,
#endif
#if AC_ACTIVE_RELAYS >= 1
153,
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
154,
#endif
#if AC_ACTIVE_RELAYS >= 1 && AC_ACTIVE_INPUTS > 0
155,
#endif
#if AC_ACTIVE_RELAYS >= 1
156,
#endif
#if AC_ACTIVE_RELAYS >= 2
157,
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
158,
#endif
#if AC_ACTIVE_RELAYS >= 2
159,
#endif
#if AC_ACTIVE_RELAYS >= 2
160,
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
161,
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
162,
#endif
#if AC_ACTIVE_RELAYS >= 2
163,
#endif
#if AC_ACTIVE_RELAYS >= 2
164,
#endif
#if AC_ACTIVE_RELAYS >= 2
165,
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
166,
#endif
#if AC_ACTIVE_RELAYS >= 2 && AC_ACTIVE_INPUTS > 0
167,
#endif
#if AC_ACTIVE_RELAYS >= 2
168,
#endif
#if AC_ACTIVE_RELAYS >= 3
169,
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
170,
#endif
#if AC_ACTIVE_RELAYS >= 3
171,
#endif
#if AC_ACTIVE_RELAYS >= 3
172,
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
173,
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
174,
#endif
#if AC_ACTIVE_RELAYS >= 3
175,
#endif
#if AC_ACTIVE_RELAYS >= 3
176,
#endif
#if AC_ACTIVE_RELAYS >= 3
177,
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
178,
#endif
#if AC_ACTIVE_RELAYS >= 3 && AC_ACTIVE_INPUTS > 0
179,
#endif
#if AC_ACTIVE_RELAYS >= 3
180,
#endif
#if AC_ACTIVE_RELAYS >= 4
181,
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
182,
#endif
#if AC_ACTIVE_RELAYS >= 4
183,
#endif
#if AC_ACTIVE_RELAYS >= 4
184,
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
185,
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
186,
#endif
#if AC_ACTIVE_RELAYS >= 4
187,
#endif
#if AC_ACTIVE_RELAYS >= 4
188,
#endif
#if AC_ACTIVE_RELAYS >= 4
189,
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
190,
#endif
#if AC_ACTIVE_RELAYS >= 4 && AC_ACTIVE_INPUTS > 0
191,
#endif
#if AC_ACTIVE_RELAYS >= 4
192,
#endif
#if 1
193,
#endif
#if AC_ACTIVE_INPUTS >= 1
194,
#endif
#if AC_ACTIVE_INPUTS >= 1
195,
#endif
#if AC_ACTIVE_INPUTS >= 1
196,
#endif
#if AC_ACTIVE_INPUTS >= 1
197,
#endif
#if AC_ACTIVE_INPUTS >= 1
198,
#endif
#if AC_ACTIVE_INPUTS >= 1
199,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
200,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
201,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
202,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
203,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
204,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
205,
#endif
#if AC_ACTIVE_INPUTS >= 1
206,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
207,
#endif
#if AC_ACTIVE_INPUTS >= 1
208,
#endif
#if AC_ACTIVE_INPUTS >= 1
209,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
210,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
211,
#endif
#if AC_ACTIVE_INPUTS >= 1
212,
#endif
#if AC_ACTIVE_INPUTS >= 1 && AC_ENABLE_PERIPHERALS
213,
#endif
#if AC_ACTIVE_INPUTS >= 1
214,
#endif
#if AC_ACTIVE_INPUTS >= 1
215,
#endif
#if AC_ACTIVE_INPUTS >= 2
216,
#endif
#if AC_ACTIVE_INPUTS >= 2
217,
#endif
#if AC_ACTIVE_INPUTS >= 2
218,
#endif
#if AC_ACTIVE_INPUTS >= 2
219,
#endif
#if AC_ACTIVE_INPUTS >= 2
220,
#endif
#if AC_ACTIVE_INPUTS >= 2
221,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
222,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
223,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
224,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
225,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
226,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
227,
#endif
#if AC_ACTIVE_INPUTS >= 2
228,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
229,
#endif
#if AC_ACTIVE_INPUTS >= 2
230,
#endif
#if AC_ACTIVE_INPUTS >= 2
231,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
232,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
233,
#endif
#if AC_ACTIVE_INPUTS >= 2
234,
#endif
#if AC_ACTIVE_INPUTS >= 2 && AC_ENABLE_PERIPHERALS
235,
#endif
#if AC_ACTIVE_INPUTS >= 2
236,
#endif
#if AC_ACTIVE_INPUTS >= 2
237,
#endif
#if AC_ACTIVE_INPUTS >= 3
238,
#endif
#if AC_ACTIVE_INPUTS >= 3
239,
#endif
#if AC_ACTIVE_INPUTS >= 3
240,
#endif
#if AC_ACTIVE_INPUTS >= 3
241,
#endif
#if AC_ACTIVE_INPUTS >= 3
242,
#endif
#if AC_ACTIVE_INPUTS >= 3
243,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
244,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
245,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
246,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
247,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
248,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
249,
#endif
#if AC_ACTIVE_INPUTS >= 3
250,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
251,
#endif
#if AC_ACTIVE_INPUTS >= 3
252,
#endif
#if AC_ACTIVE_INPUTS >= 3
253,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
254,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
255,
#endif
#if AC_ACTIVE_INPUTS >= 3
256,
#endif
#if AC_ACTIVE_INPUTS >= 3 && AC_ENABLE_PERIPHERALS
257,
#endif
#if AC_ACTIVE_INPUTS >= 3
258,
#endif
#if AC_ACTIVE_INPUTS >= 3
259,
#endif
#if AC_ACTIVE_INPUTS >= 4
260,
#endif
#if AC_ACTIVE_INPUTS >= 4
261,
#endif
#if AC_ACTIVE_INPUTS >= 4
262,
#endif
#if AC_ACTIVE_INPUTS >= 4
263,
#endif
#if AC_ACTIVE_INPUTS >= 4
264,
#endif
#if AC_ACTIVE_INPUTS >= 4
265,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
266,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
267,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
268,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
269,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
270,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
271,
#endif
#if AC_ACTIVE_INPUTS >= 4
272,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
273,
#endif
#if AC_ACTIVE_INPUTS >= 4
274,
#endif
#if AC_ACTIVE_INPUTS >= 4
275,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
276,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
277,
#endif
#if AC_ACTIVE_INPUTS >= 4
278,
#endif
#if AC_ACTIVE_INPUTS >= 4 && AC_ENABLE_PERIPHERALS
279,
#endif
#if AC_ACTIVE_INPUTS >= 4
280,
#endif
#if AC_ACTIVE_INPUTS >= 4
281,
#endif
#if AC_ACTIVE_INPUTS >= 5
282,
#endif
#if AC_ACTIVE_INPUTS >= 5
283,
#endif
#if AC_ACTIVE_INPUTS >= 5
284,
#endif
#if AC_ACTIVE_INPUTS >= 5
285,
#endif
#if AC_ACTIVE_INPUTS >= 5
286,
#endif
#if AC_ACTIVE_INPUTS >= 5
287,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
288,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
289,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
290,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
291,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
292,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
293,
#endif
#if AC_ACTIVE_INPUTS >= 5
294,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
295,
#endif
#if AC_ACTIVE_INPUTS >= 5
296,
#endif
#if AC_ACTIVE_INPUTS >= 5
297,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
298,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
299,
#endif
#if AC_ACTIVE_INPUTS >= 5
300,
#endif
#if AC_ACTIVE_INPUTS >= 5 && AC_ENABLE_PERIPHERALS
301,
#endif
#if AC_ACTIVE_INPUTS >= 5
302,
#endif
#if AC_ACTIVE_INPUTS >= 5
303,
#endif
#if AC_ACTIVE_INPUTS >= 6
304,
#endif
#if AC_ACTIVE_INPUTS >= 6
305,
#endif
#if AC_ACTIVE_INPUTS >= 6
306,
#endif
#if AC_ACTIVE_INPUTS >= 6
307,
#endif
#if AC_ACTIVE_INPUTS >= 6
308,
#endif
#if AC_ACTIVE_INPUTS >= 6
309,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
310,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
311,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
312,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
313,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
314,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
315,
#endif
#if AC_ACTIVE_INPUTS >= 6
316,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
317,
#endif
#if AC_ACTIVE_INPUTS >= 6
318,
#endif
#if AC_ACTIVE_INPUTS >= 6
319,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
320,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
321,
#endif
#if AC_ACTIVE_INPUTS >= 6
322,
#endif
#if AC_ACTIVE_INPUTS >= 6 && AC_ENABLE_PERIPHERALS
323,
#endif
#if AC_ACTIVE_INPUTS >= 6
324,
#endif
#if AC_ACTIVE_INPUTS >= 6
325,
#endif
#if AC_ACTIVE_INPUTS >= 7
326,
#endif
#if AC_ACTIVE_INPUTS >= 7
327,
#endif
#if AC_ACTIVE_INPUTS >= 7
328,
#endif
#if AC_ACTIVE_INPUTS >= 7
329,
#endif
#if AC_ACTIVE_INPUTS >= 7
330,
#endif
#if AC_ACTIVE_INPUTS >= 7
331,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
332,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
333,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
334,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
335,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
336,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
337,
#endif
#if AC_ACTIVE_INPUTS >= 7
338,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
339,
#endif
#if AC_ACTIVE_INPUTS >= 7
340,
#endif
#if AC_ACTIVE_INPUTS >= 7
341,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
342,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
343,
#endif
#if AC_ACTIVE_INPUTS >= 7
344,
#endif
#if AC_ACTIVE_INPUTS >= 7 && AC_ENABLE_PERIPHERALS
345,
#endif
#if AC_ACTIVE_INPUTS >= 7
346,
#endif
#if AC_ACTIVE_INPUTS >= 7
347,
#endif
#if AC_ACTIVE_INPUTS >= 8
348,
#endif
#if AC_ACTIVE_INPUTS >= 8
349,
#endif
#if AC_ACTIVE_INPUTS >= 8
350,
#endif
#if AC_ACTIVE_INPUTS >= 8
351,
#endif
#if AC_ACTIVE_INPUTS >= 8
352,
#endif
#if AC_ACTIVE_INPUTS >= 8
353,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
354,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
355,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
356,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
357,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
358,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
359,
#endif
#if AC_ACTIVE_INPUTS >= 8
360,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
361,
#endif
#if AC_ACTIVE_INPUTS >= 8
362,
#endif
#if AC_ACTIVE_INPUTS >= 8
363,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
364,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
365,
#endif
#if AC_ACTIVE_INPUTS >= 8
366,
#endif
#if AC_ACTIVE_INPUTS >= 8 && AC_ENABLE_PERIPHERALS
367,
#endif
#if AC_ACTIVE_INPUTS >= 8
368,
#endif
#if AC_ACTIVE_INPUTS >= 8
369,
#endif
#if 1
370,
#endif
#if 1
371,
#endif
#if 1
372,
#endif
#if 1
373,
#endif
#if 1
374,
#endif
#if 1
375,
#endif
#if 1
376,
#endif
#if AC_ACTIVE_DIAGNOSTICS
377,
#endif
#if 1
378,
#endif
#if 1
379,
#endif
#if 1
380,
#endif
#if 1
381,
#endif
#if 1
382,
#endif
#if 1
383,
#endif
#if 1
384,
#endif
#if 1
385,
#endif
#if 1
386,
#endif
#if 1
387,
#endif
#if 1
388,
#endif
#if 1
389,
#endif
#if 1
390,
#endif
#if 1
391,
#endif
#if 1
392,
#endif
#if 1
393,
#endif
#if 1
394,
#endif
#if 1
395,
#endif
#if 1
396,
#endif
};
static const uint16_t AC_TELEMETRY_COUNT=sizeof(AC_ACTIVE_TELEMETRY_IDS)/sizeof(AC_ACTIVE_TELEMETRY_IDS[0]);
static uint16_t acTelemetryIndex(uint16_t offset){
 uint16_t id=offset+1;int lo=0,hi=AC_TELEMETRY_COUNT-1;
 while(lo<=hi){int mid=(lo+hi)/2;uint16_t candidate=pgm_read_word(AC_ACTIVE_TELEMETRY_IDS+mid);
  if(candidate==id)return mid;if(candidate<id)lo=mid+1;else hi=mid-1;
 }
 return UINT16_MAX;
}
#endif
class ActiveTelemetryCache {
 String values[AC_TELEMETRY_COUNT];
#if !(AC_ENABLE_PERIPHERALS && AC_ACTIVE_RELAYS==4 && AC_ACTIVE_PWM==6 && AC_ACTIVE_INPUTS==8 && AC_ENABLE_DHT && AC_ENABLE_RTC && AC_ENABLE_BUZZER && AC_ACTIVE_FAN)
 String unsupported;
#endif
public:
 String& operator[](uint16_t offset){
#if AC_ENABLE_PERIPHERALS && AC_ACTIVE_RELAYS==4 && AC_ACTIVE_PWM==6 && AC_ACTIVE_INPUTS==8 && AC_ENABLE_DHT && AC_ENABLE_RTC && AC_ENABLE_BUZZER && AC_ACTIVE_FAN
  return values[offset];
#else
  uint16_t i=acTelemetryIndex(offset);return i==UINT16_MAX?unsupported:values[i];
#endif
 }
};
