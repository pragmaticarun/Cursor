/*
 * data.js
 * -------
 * All learning content for the "Learn the Indian Constitution" web app.
 * Everything here is plain data so the app stays a dependency-free static site.
 *
 * Structure:
 *   constitutionFacts  -> quick facts shown on the Overview page
 *   parts              -> the 25 Parts of the Constitution with descriptions
 *   schedules          -> the 12 Schedules
 *   articles           -> curated, detailed explanations of key Articles
 *   flashcards         -> question/answer pairs for the Flashcards page
 *   quizzes            -> multiple-choice questions for the Quiz page
 */

const constitutionFacts = [
  { label: "Adopted", value: "26 November 1949" },
  { label: "Came into force", value: "26 January 1950 (Republic Day)" },
  { label: "Articles (original)", value: "395 articles, 22 parts, 8 schedules" },
  { label: "Articles (current)", value: "~448 articles, 25 parts, 12 schedules" },
  { label: "Drafting Committee Chairman", value: "Dr. B. R. Ambedkar" },
  { label: "Constituent Assembly President", value: "Dr. Rajendra Prasad" },
  { label: "Time taken to draft", value: "2 years, 11 months, 18 days" },
  { label: "Nature", value: "Longest written constitution of any sovereign nation" },
];

const preamble = `WE, THE PEOPLE OF INDIA, having solemnly resolved to constitute India into a
SOVEREIGN SOCIALIST SECULAR DEMOCRATIC REPUBLIC and to secure to all its citizens:
JUSTICE, social, economic and political;
LIBERTY of thought, expression, belief, faith and worship;
EQUALITY of status and of opportunity;
and to promote among them all
FRATERNITY assuring the dignity of the individual and the unity and integrity of the Nation;
IN OUR CONSTITUENT ASSEMBLY this twenty-sixth day of November, 1949, do HEREBY ADOPT, ENACT AND GIVE TO OURSELVES THIS CONSTITUTION.`;

const preambleNotes = [
  "The words 'SOCIALIST', 'SECULAR' and 'INTEGRITY' were added by the 42nd Amendment Act, 1976.",
  "The Preamble is based on the 'Objectives Resolution' moved by Jawaharlal Nehru.",
  "In Kesavananda Bharati (1973) and Berubari Union (1960), the Supreme Court clarified that the Preamble is part of the Constitution but is not enforceable in a court of law.",
  "It declares the source of authority of the Constitution to be the people of India.",
];

/* ----------------------------- PARTS ----------------------------- */
const parts = [
  { part: "Part I", title: "The Union and its Territory", articles: "1-4",
    summary: "Defines India as a 'Union of States', describes the territory of India, and gives Parliament the power to admit, establish or reorganise states and alter their boundaries." },
  { part: "Part II", title: "Citizenship", articles: "5-11",
    summary: "Deals with citizenship at the commencement of the Constitution. The details of acquiring and losing citizenship are left to Parliament (Citizenship Act, 1955)." },
  { part: "Part III", title: "Fundamental Rights", articles: "12-35",
    summary: "The 'Magna Carta' of India. Guarantees justiciable rights such as equality, freedom, protection against exploitation, freedom of religion, cultural & educational rights, and constitutional remedies." },
  { part: "Part IV", title: "Directive Principles of State Policy", articles: "36-51",
    summary: "Non-justiciable guidelines for the State to establish a just social and economic order. Inspired by the Irish Constitution." },
  { part: "Part IVA", title: "Fundamental Duties", articles: "51A",
    summary: "Added by the 42nd Amendment (1976). Lists the duties of every citizen. Originally 10, now 11 duties (the 11th added by the 86th Amendment, 2002)." },
  { part: "Part V", title: "The Union (Central Government)", articles: "52-151",
    summary: "Covers the Executive (President, Vice-President, Council of Ministers, Attorney General), Parliament, the Union legislature, and the Supreme Court & CAG." },
  { part: "Part VI", title: "The States", articles: "152-237",
    summary: "Covers the structure of state governments: Governor, Council of Ministers, State Legislature, and High Courts." },
  { part: "Part VII", title: "(Repealed)", articles: "—",
    summary: "Originally dealt with Part B states. Repealed by the 7th Amendment Act, 1956." },
  { part: "Part VIII", title: "The Union Territories", articles: "239-242",
    summary: "Administration of Union Territories, including provisions for a Lieutenant Governor and special arrangements for Delhi." },
  { part: "Part IX", title: "The Panchayats", articles: "243-243O",
    summary: "Added by the 73rd Amendment (1992). Provides constitutional status to Panchayati Raj institutions (rural local self-government)." },
  { part: "Part IXA", title: "The Municipalities", articles: "243P-243ZG",
    summary: "Added by the 74th Amendment (1992). Provides constitutional status to urban local bodies (Municipalities)." },
  { part: "Part IXB", title: "The Co-operative Societies", articles: "243ZH-243ZT",
    summary: "Added by the 97th Amendment (2011). Deals with the incorporation, regulation and winding up of co-operative societies." },
  { part: "Part X", title: "The Scheduled and Tribal Areas", articles: "244-244A",
    summary: "Provides for the administration of Scheduled Areas and Tribal Areas (Fifth and Sixth Schedules)." },
  { part: "Part XI", title: "Relations between the Union and the States", articles: "245-263",
    summary: "Distributes legislative and administrative powers between the Centre and the States." },
  { part: "Part XII", title: "Finance, Property, Contracts and Suits", articles: "264-300A",
    summary: "Deals with financial relations, distribution of taxes, the Finance Commission, and the right to property (Art 300A)." },
  { part: "Part XIII", title: "Trade, Commerce and Intercourse", articles: "301-307",
    summary: "Guarantees freedom of trade, commerce and intercourse throughout the territory of India." },
  { part: "Part XIV", title: "Services under the Union and the States", articles: "308-323",
    summary: "Covers public services and the Public Service Commissions (UPSC and SPSCs)." },
  { part: "Part XIVA", title: "Tribunals", articles: "323A-323B",
    summary: "Added by the 42nd Amendment (1976). Provides for administrative tribunals and tribunals for other matters." },
  { part: "Part XV", title: "Elections", articles: "324-329A",
    summary: "Provides for the Election Commission of India and the conduct of free and fair elections." },
  { part: "Part XVI", title: "Special Provisions for Certain Classes", articles: "330-342A",
    summary: "Reservation of seats and special provisions for SCs, STs, Anglo-Indians (now lapsed) and backward classes." },
  { part: "Part XVII", title: "Official Language", articles: "343-351",
    summary: "Declares Hindi in Devanagari script as the official language of the Union, with English continuing for official purposes." },
  { part: "Part XVIII", title: "Emergency Provisions", articles: "352-360",
    summary: "Provides for three kinds of emergency: National (352), President's Rule in states (356), and Financial (360)." },
  { part: "Part XIX", title: "Miscellaneous", articles: "361-367",
    summary: "Protection of the President and Governors, and other miscellaneous provisions." },
  { part: "Part XX", title: "Amendment of the Constitution", articles: "368",
    summary: "Gives Parliament the power to amend the Constitution, subject to the 'basic structure' doctrine." },
  { part: "Part XXI", title: "Temporary, Transitional and Special Provisions", articles: "369-392",
    summary: "Includes special provisions for certain states (e.g., the now-abrogated Article 370 for Jammu & Kashmir)." },
  { part: "Part XXII", title: "Short Title, Commencement, Authoritative Text & Repeals", articles: "393-395",
    summary: "Final provisions: short title, date of commencement, authoritative Hindi text, and repeals." },
];

/* --------------------------- SCHEDULES --------------------------- */
const schedules = [
  { schedule: "First Schedule", title: "States and Union Territories",
    detail: "Lists the States and Union Territories of India and their territories." },
  { schedule: "Second Schedule", title: "Salaries & Emoluments",
    detail: "Provisions on the salaries, allowances and privileges of the President, Governors, judges, the CAG, etc." },
  { schedule: "Third Schedule", title: "Forms of Oaths and Affirmations",
    detail: "Forms of oaths/affirmations for ministers, MPs/MLAs, judges and the CAG." },
  { schedule: "Fourth Schedule", title: "Allocation of Rajya Sabha Seats",
    detail: "Allocates seats in the Rajya Sabha (Council of States) to states and union territories." },
  { schedule: "Fifth Schedule", title: "Administration of Scheduled Areas",
    detail: "Administration and control of Scheduled Areas and Scheduled Tribes." },
  { schedule: "Sixth Schedule", title: "Tribal Areas",
    detail: "Administration of tribal areas in Assam, Meghalaya, Tripura and Mizoram via Autonomous District Councils." },
  { schedule: "Seventh Schedule", title: "Union, State & Concurrent Lists",
    detail: "Divides legislative powers between the Union and the States through three lists." },
  { schedule: "Eighth Schedule", title: "Languages",
    detail: "Lists the 22 official languages recognised by the Constitution." },
  { schedule: "Ninth Schedule", title: "Validation of Certain Acts",
    detail: "Added by the 1st Amendment (1951) to protect land-reform laws from judicial review (now reviewable for basic structure violations)." },
  { schedule: "Tenth Schedule", title: "Anti-Defection Law",
    detail: "Added by the 52nd Amendment (1985). Provides for disqualification of members on grounds of defection." },
  { schedule: "Eleventh Schedule", title: "Powers of Panchayats",
    detail: "Added by the 73rd Amendment (1992). Lists 29 functional subjects of Panchayats." },
  { schedule: "Twelfth Schedule", title: "Powers of Municipalities",
    detail: "Added by the 74th Amendment (1992). Lists 18 functional subjects of Municipalities." },
];

/* ---------------------------- ARTICLES --------------------------- */
/* A curated set of the most important articles, each with a detailed
 * explanation. `part` ties an article back to a Part for filtering. */
const articles = [
  {
    number: "Article 1", title: "Name and territory of the Union", part: "Part I",
    tags: ["Union", "Territory"],
    text: "India, that is Bharat, shall be a Union of States. The territory of India comprises the territories of the States, the Union Territories, and any other territories that may be acquired.",
    explanation: "Article 1 describes India as a 'Union of States' rather than a 'Federation of States'. Dr. Ambedkar explained that the word 'Union' was deliberately chosen to convey two ideas: (1) the Indian federation is not the result of an agreement among states, and (2) no state has the right to secede from the Union. India is thus an indestructible union of destructible states — Parliament can reorganise states, but states cannot leave the Union."
  },
  {
    number: "Article 3", title: "Formation of new States and alteration of areas", part: "Part I",
    tags: ["Reorganisation"],
    text: "Parliament may by law form a new State, increase or diminish the area of any State, or alter the boundaries or name of any State.",
    explanation: "This article gives Parliament wide power to redraw the political map of India by a simple majority. The President must refer the bill to the affected state legislature for its views, but Parliament is not bound by those views. This power has been used many times — e.g., the States Reorganisation Act of 1956 and the creation of states like Telangana (2014)."
  },
  {
    number: "Article 12", title: "Definition of 'State'", part: "Part III",
    tags: ["Fundamental Rights"],
    text: "In Part III, unless the context otherwise requires, 'the State' includes the Government and Parliament of India, the Government and Legislature of each State, and all local or other authorities within the territory of India or under the control of the Government of India.",
    explanation: "Article 12 defines the term 'State' for the purpose of Fundamental Rights, because most rights in Part III are enforceable against the State. Courts have interpreted 'other authorities' broadly to include statutory bodies, government companies and any body that is an 'instrumentality or agency of the State'."
  },
  {
    number: "Article 13", title: "Laws inconsistent with Fundamental Rights", part: "Part III",
    tags: ["Fundamental Rights", "Judicial Review"],
    text: "All laws in force that are inconsistent with the Fundamental Rights are void to the extent of inconsistency. The State shall not make any law that takes away or abridges these rights.",
    explanation: "Article 13 is the constitutional basis for the power of judicial review in matters of Fundamental Rights. It declares that any law violating Part III is void. It applies the doctrines of severability (only the offending portion is struck down) and eclipse (a pre-Constitution law becomes dormant rather than dead)."
  },
  {
    number: "Article 14", title: "Equality before law", part: "Part III",
    tags: ["Equality", "Fundamental Rights"],
    text: "The State shall not deny to any person equality before the law or the equal protection of the laws within the territory of India.",
    explanation: "Article 14 guarantees two things: 'equality before law' (a British concept implying the absence of special privileges) and 'equal protection of the laws' (an American concept implying equal treatment in equal circumstances). It permits 'reasonable classification' — the State may treat different groups differently if the classification is based on an intelligible differentia having a rational nexus with the objective. It forbids arbitrariness in State action."
  },
  {
    number: "Article 15", title: "Prohibition of discrimination", part: "Part III",
    tags: ["Equality", "Fundamental Rights"],
    text: "The State shall not discriminate against any citizen on grounds only of religion, race, caste, sex or place of birth.",
    explanation: "Article 15 prohibits discrimination on five specific grounds. It includes enabling provisions allowing the State to make special provisions for women, children, socially and educationally backward classes, SCs and STs (including reservations in educational institutions under Article 15(5))."
  },
  {
    number: "Article 16", title: "Equality of opportunity in public employment", part: "Part III",
    tags: ["Equality", "Reservation"],
    text: "There shall be equality of opportunity for all citizens in matters of public employment. The State may make provision for the reservation of appointments for backward classes not adequately represented.",
    explanation: "Article 16 guarantees equal opportunity in government jobs while permitting reservations for backward classes, SCs and STs. The landmark Indra Sawhney case (1992) upheld reservations but capped them at 50% (subject to exceptions) and introduced the 'creamy layer' exclusion."
  },
  {
    number: "Article 17", title: "Abolition of Untouchability", part: "Part III",
    tags: ["Equality"],
    text: "'Untouchability' is abolished and its practice in any form is forbidden. Enforcing any disability arising out of 'untouchability' is an offence punishable by law.",
    explanation: "Article 17 abolishes untouchability — one of the few rights enforceable against private individuals, not just the State. It is given effect through the Protection of Civil Rights Act, 1955 and the SC/ST (Prevention of Atrocities) Act, 1989."
  },
  {
    number: "Article 19", title: "Six Freedoms", part: "Part III",
    tags: ["Freedom", "Fundamental Rights"],
    text: "All citizens have the right to (a) freedom of speech and expression; (b) assemble peaceably and without arms; (c) form associations or unions; (d) move freely throughout India; (e) reside and settle in any part of India; (g) practise any profession, or carry on any occupation, trade or business.",
    explanation: "Article 19 guarantees six fundamental freedoms to citizens. Each freedom can be restricted only by 'reasonable restrictions' on specific grounds listed in the article (e.g., security of the State, public order, decency, morality). The original right to property under 19(1)(f) was removed by the 44th Amendment (1978)."
  },
  {
    number: "Article 21", title: "Protection of life and personal liberty", part: "Part III",
    tags: ["Life", "Liberty", "Fundamental Rights"],
    text: "No person shall be deprived of his life or personal liberty except according to procedure established by law.",
    explanation: "Article 21 is the heart of fundamental rights. After Maneka Gandhi v. Union of India (1978), the procedure must be 'just, fair and reasonable', not arbitrary. The Supreme Court has expanded Article 21 to include the right to privacy (Puttaswamy, 2017), the right to a clean environment, livelihood, health, shelter, dignity, speedy trial and education."
  },
  {
    number: "Article 21A", title: "Right to Education", part: "Part III",
    tags: ["Education", "Fundamental Rights"],
    text: "The State shall provide free and compulsory education to all children of the age of six to fourteen years in such manner as the State may determine.",
    explanation: "Article 21A was inserted by the 86th Amendment Act, 2002, making elementary education a fundamental right. It is implemented through the Right of Children to Free and Compulsory Education (RTE) Act, 2009."
  },
  {
    number: "Article 22", title: "Protection against arrest and detention", part: "Part III",
    tags: ["Liberty", "Fundamental Rights"],
    text: "Provides safeguards against arbitrary arrest and detention, including the right to be informed of grounds of arrest, to consult a lawyer, and to be produced before a magistrate within 24 hours.",
    explanation: "Article 22 has two parts: protections for ordinary arrests, and a separate set of (weaker) safeguards for preventive detention. Preventive detention allows detention without trial to prevent future offences, subject to review by an Advisory Board."
  },
  {
    number: "Article 23", title: "Prohibition of trafficking and forced labour", part: "Part III",
    tags: ["Exploitation", "Fundamental Rights"],
    text: "Traffic in human beings, begar (forced labour) and other similar forms of forced labour are prohibited and any contravention is an offence.",
    explanation: "Article 23 protects against human trafficking and forced labour. It is enforceable against both the State and private persons. The State may impose compulsory service for public purposes (e.g., military service) without discrimination."
  },
  {
    number: "Article 24", title: "Prohibition of child labour", part: "Part III",
    tags: ["Exploitation", "Children"],
    text: "No child below the age of fourteen years shall be employed in any factory or mine or engaged in any other hazardous employment.",
    explanation: "Article 24 prohibits the employment of children below 14 in factories, mines and hazardous work. It is supported by laws such as the Child Labour (Prohibition and Regulation) Act, 1986."
  },
  {
    number: "Article 25", title: "Freedom of religion", part: "Part III",
    tags: ["Religion", "Fundamental Rights"],
    text: "All persons are equally entitled to freedom of conscience and the right freely to profess, practise and propagate religion, subject to public order, morality and health.",
    explanation: "Article 25 guarantees freedom of conscience and religion to all persons (not just citizens). It is subject to public order, morality, health and other fundamental rights. The State can regulate secular activities associated with religion and provide for social welfare and reform."
  },
  {
    number: "Article 32", title: "Right to Constitutional Remedies", part: "Part III",
    tags: ["Remedies", "Writs", "Fundamental Rights"],
    text: "The right to move the Supreme Court for the enforcement of Fundamental Rights is guaranteed. The Supreme Court may issue writs — habeas corpus, mandamus, prohibition, certiorari and quo warranto.",
    explanation: "Dr. Ambedkar called Article 32 'the heart and soul of the Constitution' because it makes Fundamental Rights meaningful by guaranteeing a remedy. It is itself a Fundamental Right. The five writs are powerful tools to protect citizens against unlawful State action. (High Courts have similar power under Article 226, which is wider.)"
  },
  {
    number: "Article 36-51", title: "Directive Principles of State Policy", part: "Part IV",
    tags: ["DPSP"],
    text: "Directives to the State to secure a social order based on justice, adequate means of livelihood, equal pay for equal work, free legal aid, organisation of village panchayats, the right to work, a uniform civil code, protection of the environment and promotion of international peace.",
    explanation: "Part IV (Articles 36-51) contains the Directive Principles — non-justiciable goals borrowed from the Irish Constitution. Though not enforceable in court, they are 'fundamental in the governance of the country'. They are classified as Socialistic (e.g., Art 39, 41, 43), Gandhian (e.g., Art 40, 43, 46) and Liberal-Intellectual (e.g., Art 44, 48, 51) principles."
  },
  {
    number: "Article 40", title: "Organisation of village panchayats", part: "Part IV",
    tags: ["DPSP", "Gandhian"],
    text: "The State shall take steps to organise village panchayats and endow them with such powers and authority as may be necessary to enable them to function as units of self-government.",
    explanation: "A Gandhian Directive Principle reflecting the ideal of Gram Swaraj (village self-rule). It was given concrete shape decades later through the 73rd Amendment Act, 1992."
  },
  {
    number: "Article 44", title: "Uniform Civil Code", part: "Part IV",
    tags: ["DPSP"],
    text: "The State shall endeavour to secure for the citizens a uniform civil code throughout the territory of India.",
    explanation: "Article 44 expresses the goal of a common set of personal laws (marriage, divorce, inheritance) for all citizens regardless of religion. It remains a directive principle and a subject of ongoing public debate."
  },
  {
    number: "Article 50", title: "Separation of judiciary from executive", part: "Part IV",
    tags: ["DPSP"],
    text: "The State shall take steps to separate the judiciary from the executive in the public services of the State.",
    explanation: "This principle aims to ensure an independent judiciary by separating judicial functions from executive control, a goal largely achieved through the Code of Criminal Procedure, 1973."
  },
  {
    number: "Article 51A", title: "Fundamental Duties", part: "Part IVA",
    tags: ["Duties"],
    text: "Lists the eleven fundamental duties of every citizen, including to abide by the Constitution, cherish the freedom struggle, protect sovereignty, promote harmony, preserve the environment, develop scientific temper, and provide opportunities for education to one's child (the 11th duty).",
    explanation: "Fundamental Duties were added by the 42nd Amendment (1976) on the recommendation of the Swaran Singh Committee, inspired by the Soviet Constitution. Originally ten, the 86th Amendment (2002) added the duty of every parent/guardian to provide education to children aged 6-14. Duties are not legally enforceable but serve as a constant reminder of civic responsibility."
  },
  {
    number: "Article 52-53", title: "The President of India", part: "Part V",
    tags: ["Executive", "President"],
    text: "There shall be a President of India in whom the executive power of the Union is vested and exercised by him directly or through officers subordinate to him in accordance with the Constitution.",
    explanation: "The President is the constitutional head of the Union executive. Real executive power is exercised by the Council of Ministers headed by the Prime Minister (Article 74), whose advice is binding on the President (after the 42nd and 44th Amendments). The President is elected indirectly by an electoral college (Article 54)."
  },
  {
    number: "Article 72", title: "Pardoning power of the President", part: "Part V",
    tags: ["Executive", "President"],
    text: "The President has the power to grant pardons, reprieves, respites or remissions of punishment, or to suspend, remit or commute a sentence, in certain cases including all death sentences.",
    explanation: "Article 72 gives the President clemency powers, which act as a check against possible judicial errors and as an instrument of mercy. Governors have a similar but narrower power under Article 161 (Governors cannot pardon a death sentence or punishments by court-martial)."
  },
  {
    number: "Article 74-75", title: "Council of Ministers and Prime Minister", part: "Part V",
    tags: ["Executive"],
    text: "There shall be a Council of Ministers headed by the Prime Minister to aid and advise the President, who shall act in accordance with such advice. The Prime Minister is appointed by the President; other ministers are appointed on the PM's advice.",
    explanation: "Articles 74 and 75 establish the parliamentary system of government in India. The Council of Ministers is collectively responsible to the Lok Sabha (Article 75(3)). The 91st Amendment (2003) capped the size of the Council of Ministers at 15% of the strength of the Lok Sabha."
  },
  {
    number: "Article 79-100", title: "Parliament", part: "Part V",
    tags: ["Legislature"],
    text: "Parliament consists of the President, the Council of States (Rajya Sabha) and the House of the People (Lok Sabha). These articles cover its composition, sessions, officers, and the conduct of business.",
    explanation: "India has a bicameral Parliament. The Lok Sabha (lower house) is directly elected and is the more powerful house, especially in money matters. The Rajya Sabha (upper house) represents the states and is a permanent body that is never fully dissolved (one-third of members retire every two years)."
  },
  {
    number: "Article 110", title: "Money Bills", part: "Part V",
    tags: ["Legislature", "Finance"],
    text: "Defines a Money Bill as one dealing only with matters such as taxation, government borrowing, and the Consolidated Fund of India. The Speaker of the Lok Sabha certifies whether a bill is a Money Bill.",
    explanation: "Money Bills can be introduced only in the Lok Sabha and only on the President's recommendation. The Rajya Sabha can only suggest changes within 14 days but cannot reject a Money Bill, giving the Lok Sabha decisive control over finances."
  },
  {
    number: "Article 124", title: "Establishment of the Supreme Court", part: "Part V",
    tags: ["Judiciary"],
    text: "There shall be a Supreme Court of India consisting of a Chief Justice and other judges. Judges are appointed by the President and hold office until the age of 65.",
    explanation: "Article 124 establishes the Supreme Court as the apex court and guardian of the Constitution. Judicial independence is protected through security of tenure, fixed salaries charged on the Consolidated Fund, and a difficult removal process (impeachment for 'proved misbehaviour or incapacity')."
  },
  {
    number: "Article 143", title: "Advisory jurisdiction of the Supreme Court", part: "Part V",
    tags: ["Judiciary"],
    text: "The President may refer a question of law or fact of public importance to the Supreme Court for its opinion.",
    explanation: "Article 143 gives the Supreme Court advisory (consultative) jurisdiction. The opinion is not binding on the President and is not a judgment, but it carries great persuasive value. It has been used in cases like the Berubari and Ayodhya references."
  },
  {
    number: "Article 148", title: "Comptroller and Auditor-General (CAG)", part: "Part V",
    tags: ["Audit"],
    text: "There shall be a Comptroller and Auditor-General of India who audits the accounts of the Union and the States and reports to the legislatures.",
    explanation: "The CAG is the guardian of the public purse and one of the bulwarks of the democratic system. Dr. Ambedkar described the CAG as one of the most important officers under the Constitution. The CAG's reports are examined by the Public Accounts Committee."
  },
  {
    number: "Article 153-162", title: "The Governor of a State", part: "Part VI",
    tags: ["Executive", "States"],
    text: "There shall be a Governor for each State who is the constitutional head of the State executive, appointed by the President and holding office during the President's pleasure.",
    explanation: "The Governor is the state counterpart of the President. The Governor acts on the advice of the State Council of Ministers but also has certain discretionary powers (e.g., reserving bills for the President, reporting on the failure of constitutional machinery)."
  },
  {
    number: "Article 226", title: "Power of High Courts to issue writs", part: "Part VI",
    tags: ["Judiciary", "Writs"],
    text: "Every High Court has the power to issue writs for the enforcement of Fundamental Rights and for any other purpose.",
    explanation: "Article 226 gives High Courts a wider writ jurisdiction than the Supreme Court under Article 32, because High Courts can issue writs not only for Fundamental Rights but for 'any other purpose' (i.e., ordinary legal rights too). Unlike Article 32, the right to approach a High Court is not itself a Fundamental Right."
  },
  {
    number: "Article 243", title: "Panchayati Raj", part: "Part IX",
    tags: ["Local Government"],
    text: "Provides for a three-tier system of Panchayats at the village, intermediate and district levels, with regular elections, reservation of seats, and a State Finance Commission.",
    explanation: "Part IX, added by the 73rd Amendment (1992), gave constitutional status to rural local self-government. It mandates elections every five years, reservation for SCs, STs and women (at least one-third), and the establishment of State Election Commissions and State Finance Commissions."
  },
  {
    number: "Article 280", title: "Finance Commission", part: "Part XII",
    tags: ["Finance"],
    text: "The President shall constitute a Finance Commission every five years to recommend the distribution of tax revenues between the Union and the States.",
    explanation: "The Finance Commission is a quasi-judicial body that addresses vertical and horizontal fiscal imbalances by recommending how taxes should be shared between the Centre and the States and among the States themselves."
  },
  {
    number: "Article 300A", title: "Right to Property", part: "Part XII",
    tags: ["Property"],
    text: "No person shall be deprived of his property save by authority of law.",
    explanation: "Originally a Fundamental Right (Articles 19(1)(f) and 31), the right to property was removed from Part III by the 44th Amendment (1978) and made a constitutional/legal right under Article 300A. The State can now acquire property by law, but the action must still be lawful and non-arbitrary."
  },
  {
    number: "Article 324", title: "Election Commission of India", part: "Part XV",
    tags: ["Elections"],
    text: "The superintendence, direction and control of elections to Parliament, State Legislatures, and the offices of President and Vice-President shall vest in an Election Commission.",
    explanation: "Article 324 establishes the Election Commission of India (ECI) as an independent constitutional body responsible for conducting free and fair elections. It is currently a multi-member body (Chief Election Commissioner and Election Commissioners)."
  },
  {
    number: "Article 352", title: "National Emergency", part: "Part XVIII",
    tags: ["Emergency"],
    text: "If the President is satisfied that the security of India is threatened by war, external aggression or armed rebellion, a Proclamation of National Emergency may be made.",
    explanation: "A National Emergency under Article 352 can be declared on grounds of war, external aggression, or armed rebellion (the term 'internal disturbance' was replaced by 'armed rebellion' by the 44th Amendment, 1978). It must be approved by both Houses within one month. The most controversial use was the 1975-77 Emergency."
  },
  {
    number: "Article 356", title: "President's Rule (State Emergency)", part: "Part XVIII",
    tags: ["Emergency"],
    text: "If the President is satisfied that the government of a State cannot be carried on in accordance with the Constitution, the President may assume the functions of the State government.",
    explanation: "Article 356 imposes 'President's Rule' when constitutional machinery fails in a state. The landmark S. R. Bommai case (1994) laid down strict guidelines to curb its misuse, including that the floor of the Assembly is the proper place to test a government's majority and that the proclamation is subject to judicial review."
  },
  {
    number: "Article 360", title: "Financial Emergency", part: "Part XVIII",
    tags: ["Emergency"],
    text: "If the President is satisfied that the financial stability or credit of India is threatened, a Proclamation of Financial Emergency may be made.",
    explanation: "Article 360 allows the Centre to direct states to observe financial propriety and reduce salaries, including those of judges. A Financial Emergency has never been declared in India to date."
  },
  {
    number: "Article 368", title: "Amendment of the Constitution", part: "Part XX",
    tags: ["Amendment", "Basic Structure"],
    text: "Parliament may amend the Constitution by way of addition, variation or repeal, following the prescribed procedure. Some amendments require a special majority; others also require ratification by half the state legislatures.",
    explanation: "Article 368 provides the amendment procedure. The Kesavananda Bharati case (1973) established the 'Basic Structure Doctrine': Parliament can amend any part of the Constitution but cannot alter its basic structure (e.g., supremacy of the Constitution, rule of law, separation of powers, federalism, secularism, judicial review and free elections)."
  },
  {
    number: "Article 370", title: "Special status for Jammu & Kashmir (abrogated)", part: "Part XXI",
    tags: ["Special Provisions"],
    text: "A 'temporary provision' that granted special autonomous status to the State of Jammu and Kashmir.",
    explanation: "Article 370 gave Jammu & Kashmir a separate constitution and significant autonomy. In August 2019, the Government effectively abrogated its special status, and the state was reorganised into two Union Territories. The Supreme Court upheld the abrogation in December 2023."
  },
];

/* --------------------------- FLASHCARDS -------------------------- */
const flashcards = [
  { front: "When did the Constitution of India come into force?", back: "26 January 1950, celebrated as Republic Day. It was adopted earlier on 26 November 1949 (Constitution Day)." },
  { front: "Who was the Chairman of the Drafting Committee?", back: "Dr. B. R. Ambedkar, often called the chief architect of the Indian Constitution." },
  { front: "Which Part contains the Fundamental Rights?", back: "Part III (Articles 12-35)." },
  { front: "Which Article is called the 'heart and soul' of the Constitution?", back: "Article 32 — the Right to Constitutional Remedies (Dr. Ambedkar's words)." },
  { front: "What are the five writs under Article 32?", back: "Habeas Corpus, Mandamus, Prohibition, Certiorari and Quo Warranto." },
  { front: "Which Amendment added the words SOCIALIST, SECULAR and INTEGRITY to the Preamble?", back: "The 42nd Amendment Act, 1976." },
  { front: "Which Article guarantees the Right to Education?", back: "Article 21A, inserted by the 86th Amendment Act, 2002 (free and compulsory education for ages 6-14)." },
  { front: "From which country was the idea of Directive Principles borrowed?", back: "Ireland (the Irish Constitution)." },
  { front: "What is the Basic Structure Doctrine?", back: "Established in Kesavananda Bharati (1973): Parliament can amend the Constitution but cannot destroy its basic structure." },
  { front: "How many Fundamental Duties are there now?", back: "Eleven. Originally ten (42nd Amendment, 1976); the 11th was added by the 86th Amendment, 2002." },
  { front: "Which Article deals with the abolition of untouchability?", back: "Article 17." },
  { front: "Which two amendments gave constitutional status to local self-government?", back: "73rd Amendment (Panchayats) and 74th Amendment (Municipalities), both in 1992." },
  { front: "What are the three types of emergencies in the Constitution?", back: "National Emergency (Art 352), President's Rule / State Emergency (Art 356), and Financial Emergency (Art 360)." },
  { front: "How many Schedules does the Constitution currently have?", back: "Twelve schedules (originally eight)." },
  { front: "Which Article was used to abrogate the special status of Jammu & Kashmir?", back: "Article 370 (effectively abrogated in August 2019)." },
  { front: "Who was the President of the Constituent Assembly?", back: "Dr. Rajendra Prasad." },
  { front: "What does Article 14 guarantee?", back: "Equality before the law and equal protection of the laws." },
  { front: "Which case expanded the meaning of 'right to life' under Article 21?", back: "Maneka Gandhi v. Union of India (1978) — procedure must be just, fair and reasonable." },
  { front: "Which Schedule contains the anti-defection law?", back: "The Tenth Schedule (added by the 52nd Amendment, 1985)." },
  { front: "Which body recommends the sharing of taxes between the Centre and States?", back: "The Finance Commission, constituted under Article 280." },
];

/* ----------------------------- QUIZZES --------------------------- */
const quizzes = [
  {
    q: "On which date did the Constitution of India come into effect?",
    options: ["15 August 1947", "26 November 1949", "26 January 1950", "2 October 1950"],
    answer: 2,
    explain: "It came into force on 26 January 1950, celebrated as Republic Day. It was adopted on 26 November 1949."
  },
  {
    q: "Who is regarded as the chief architect of the Indian Constitution?",
    options: ["Jawaharlal Nehru", "Dr. B. R. Ambedkar", "Sardar Vallabhbhai Patel", "Dr. Rajendra Prasad"],
    answer: 1,
    explain: "Dr. B. R. Ambedkar chaired the Drafting Committee and is the chief architect of the Constitution."
  },
  {
    q: "Fundamental Rights are contained in which Part of the Constitution?",
    options: ["Part II", "Part III", "Part IV", "Part V"],
    answer: 1,
    explain: "Part III (Articles 12-35) contains the Fundamental Rights."
  },
  {
    q: "Which Article is known as the 'heart and soul' of the Constitution?",
    options: ["Article 14", "Article 19", "Article 21", "Article 32"],
    answer: 3,
    explain: "Dr. Ambedkar called Article 32 (Right to Constitutional Remedies) the heart and soul of the Constitution."
  },
  {
    q: "The Directive Principles of State Policy were borrowed from the Constitution of which country?",
    options: ["United States", "United Kingdom", "Ireland", "Canada"],
    answer: 2,
    explain: "The concept of Directive Principles was borrowed from the Irish Constitution."
  },
  {
    q: "Which amendment added the words 'Socialist' and 'Secular' to the Preamble?",
    options: ["1st Amendment", "42nd Amendment", "44th Amendment", "73rd Amendment"],
    answer: 1,
    explain: "The 42nd Amendment (1976) added 'Socialist', 'Secular' and 'Integrity' to the Preamble."
  },
  {
    q: "The Basic Structure Doctrine was laid down in which landmark case?",
    options: ["Maneka Gandhi case", "Kesavananda Bharati case", "Golaknath case", "Minerva Mills case"],
    answer: 1,
    explain: "Kesavananda Bharati v. State of Kerala (1973) established the Basic Structure Doctrine."
  },
  {
    q: "How many Fundamental Duties are currently listed in the Constitution?",
    options: ["10", "11", "12", "9"],
    answer: 1,
    explain: "There are 11 Fundamental Duties (Article 51A); the 11th was added by the 86th Amendment in 2002."
  },
  {
    q: "Article 21A guarantees which right?",
    options: ["Right to Property", "Right to Education", "Right to Privacy", "Right to Information"],
    answer: 1,
    explain: "Article 21A provides the Right to Education for children aged 6 to 14 years."
  },
  {
    q: "Which Article provides for President's Rule in a State?",
    options: ["Article 352", "Article 356", "Article 360", "Article 370"],
    answer: 1,
    explain: "Article 356 deals with President's Rule (State Emergency) when constitutional machinery fails."
  },
  {
    q: "The 73rd Amendment Act of 1992 is associated with which institution?",
    options: ["Municipalities", "Panchayati Raj", "Co-operative Societies", "Tribunals"],
    answer: 1,
    explain: "The 73rd Amendment gave constitutional status to Panchayati Raj (rural local government)."
  },
  {
    q: "Which writ literally means 'to have the body'?",
    options: ["Mandamus", "Certiorari", "Habeas Corpus", "Quo Warranto"],
    answer: 2,
    explain: "Habeas Corpus means 'to have the body' and is used against unlawful detention."
  },
  {
    q: "How many Schedules does the Constitution currently contain?",
    options: ["8", "10", "12", "14"],
    answer: 2,
    explain: "There are 12 Schedules currently (originally 8)."
  },
  {
    q: "The right to property is now protected under which Article?",
    options: ["Article 19(1)(f)", "Article 31", "Article 300A", "Article 21"],
    answer: 2,
    explain: "After the 44th Amendment (1978), the right to property became a legal right under Article 300A."
  },
  {
    q: "Which body conducts elections to Parliament and State Legislatures?",
    options: ["Finance Commission", "Election Commission of India", "UPSC", "NITI Aayog"],
    answer: 1,
    explain: "Article 324 vests the conduct of elections in the Election Commission of India."
  },
];

/* Expose data globally for the non-module script. */
window.ConstitutionData = {
  constitutionFacts,
  preamble,
  preambleNotes,
  parts,
  schedules,
  articles,
  flashcards,
  quizzes,
};
