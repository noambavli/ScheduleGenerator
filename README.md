### Regular Users – Please ignore the Hebrew section below. There’s an English explanation with instructions following it. The Hebrew section is temporary.



### זה צריך עוד טסטים ובדיקות על קלטים, המטרה הייתה מימוש כדי להגיע לתוצאות ופחות חווית משתמש 
הרעיון זה מימוש האלגוריתם כדי להגיע לתוצאה רצוייה, הנראות וחווית המשתמש יכלה להיות הרבה יותר טובה אבל שוב זה לא קושי להוסיף לקוח (שפשוט יקבל וישמור מידע ויציג למשתמש) ועבור הפרוייקט של ה10 יחל אכתוב את זה גם בג'אווה עם UI מהמם.
# *%היי מראיינים , אם לא הבנתם איך זה עובד/מה מיוחד בזה כי גמגמתי ולא הסברתי טוב בכלל הנה הסבר קטן שאמור להעביר טיפה את הקטע:*
## זה מסדר מערכת לכמה כיתות במקביל עם המורים הנתונים, קודם כל כפי שניסתי להסביר וגמגמתי לא השתמשתי בבאקטראקינג כי זה פשוט היה מוצא את כל האפשרויות הקיימות , וזה יכול להיות גם אלפים, אז איך הוא ידע מה הכי טוב? סתם ייתן אופציות? לא די בכך שהקומפלקסיטי בשמיים? אז במקום להסתבך חישבתי אופציה אפשרית עבור כל השכבה בהינתן :
### 1. העדפות של גודל בלוק(בקוד בלוק=צ'אנק) שעות (למשל מורה 3 עם כיתה 4 סה"כ 5 שעות : העדפה לשיעורים של שעתיים כל פעם, כלומר 2 שיעורים שעתיים ושיעור אחד שעה )  <br>   <br> 2. העדפת מורה 1\2\3 (במידה ובסוף אין מספיק מקום/שעות/יש בעיות בלתי פתירות כיתת ספורט רוצה שקודם בטוח ישובץ מורה מספר 2 למשל) <br> <br>   3. מגבלות של זמנים לכולם/מורים/כיתות (וכך גם אפשר להגדיר ימים קצרים וכו)
## ואז זה פשוט ממלא , עם הימנעות מסטארווינג של כיתות מסויימות, והעדיפות העליונה היא שקודם ישובצו המורים שהוגדרו חשובים גם אם הבלוק נקטע (הקבלות, מורה לא עובד בשעות האלו וכו) ולבסוף יש מתקן לוגי שמאתר אפשרויות לשעות מתאימות להחלפה בין מורים וכיתות במקומות שנקטעו כדי להגיע לתוצאה אופטימלית יותר 
### ** הוספתי עוד אופציה שנייה לאלגוריתם שסתם הוספתי בסוף שעובדת שונה, מחפשת מורה אחר ישר לא צריכה ממש את המתקן הלוגי אבל הרבה פחות עונה על העדפות היא רק צריכה עוד טיפה תיקונים וטסטים אבל פחות רלוונטי , לבסוף זה גם עושה בדיקה קטנה על שעות שלא שובצו האם זה בגלל מורה מסויים וכולי ומראה את הסיבה למה (הסיבה - string סופקה כשההגבלות הוזנו )
### הקטע שזה בתוך מערך תלת מימדי זה לא כדי שזה יהיה מגניב אלא שיהיה מהיר לבדוק האם מורים/כיתות עסוקים בשעות מסויימות. המערך מורכב משעה-יום-כיתה כך שבכל תא יש מספר תלת ספרתי של יחידות ועשרות- מספר מורה, מאות- כמות שעות מועדפת (נגיד שיעורים של שלוש שעות כל פעם) , כך שמציאת המידע הרלוונטי כל פעם מתבצעת מאוד מהר + מעט ממש זיכרון. 
מקום לשיפור- דוקומנטציה , בגלל שזה היה פרוייקט לכיף העיקר שתצא תוצאה שתתאים לבית ספר (אגב גם הui מזעזע) וכולי פחות השקעתי בהאם אנשים יבינו את הקוד יותר לממש משהו שיביא פתרון למערכת בלי מאמץ מחשבתי וסידור של המשתמש
ביי יום טוב





**Schedule Generator**<br>
by Noam Bavli
<br>
Before anyone asks,
#### I know about "using namespace std", but I prefer not to use it because in large-scale projects (not like this, real ones, i just do it all the time cause i think its good practice) 
  that require a lot of libraries,I’d rather know if a function comes from the standard library or another source. This helps avoid 
  conflicts with namespaces.
  Of course, I don’t mind using it, but I think it’s better to be explicit and not rely on using namespace std
  <br>
#### i have used the template print func even tho less efficient cause i think its pretty and the ui is basically for testing and in real 
  server-clinet environment i'ts not gonna print anything anyway (the server, just gonna send the data to the client and the client print and visualize). 
<br>
<br>
This is a cool, smart schedule generator, specifically designed for schools and universities. 
The user provides teacher-class pairs, number of days (of the "week"), and the number of hours for each day (and ofc you can specify blocked times), and more preferences.
the generator creates an smart schedule.
<br>
<br>
It supports simultaneous scheduling for multiple classes, with a capacity for up to 99 teachers and no limit on the number of classes.
<br>
Key features include:
<br>
<br>
**Teacher prioritization**<br> Teacher prioritization for each teacher, for each class- For instance, in a sports class,you can prioritizes scheduling the sports teacher’s hours first.
(you can choose 1 - low/2 - mid/ 3- high level )
<br>
<br>
**Block size prioritization**<br> the generator prioritizes the chunks you choose for
each teacher, for each class- For example, if a math teacher for class x prefers four-hour blocks,the generator prioritizes that preference.
<br>
<br>
**Logical optimizer**<br> for incomplete chunks A logical optimizer for fixing incomplete chunks. For example, if a teacher is scheduled
for a chunk and it's interrupted by a blocked hour for that specific teacher, resulting in a replacement by another teacher,
the optimizer can resolve this by replacing the hours between surrounding teachers and finding suitable hours within the whole schedule for the replacements.
<br>
<br>
**Customizable block times**<br> You can set specific block times for individual classes or teachers, or apply them universally across all classes and teachers.
<br>
<br>
++Acknowledging the user if a certain teacher's blocked time results in nothing being scheduled during that time, even though it is very rare,
along with the reason provided by the teacher when filling out the blocked times.
<br>
<br>
Note: The UI is currently horrible. More testing and user input validation are required.
The primary goal of this project is to implement all the algorithms and develop a system that processes data and generates accurate schedules.
During development, I used predefined data within the code. When you're ready to use the UI, simply comment out the predefined data 
section and uncomment the UI portion.
<br>
<br>
The schedule itself is stored as a 3D array. Other data is held in pairs, lists, and vectors, which might introduce some inconsistency.
I aimed to use the best structure for each case, though it's a bit confusing and may need further refinement.
<br>
<br>
The 3D array stores three-digit numbers: the hundreds digit represents the teacher, while the tens and ones digits represent the preferred time block.
Day-hour identifiers are stored as day*24 + hour, using basic mathematical operations for extremely fast data extraction, compression, and comparison.
The patterns for storing and extracting values are simple and do not compromise reliability.
<br>
**I don’t know why I thought until the last commits that “schedule” was spelled “schedual.” I fixed it in the last commits in the files and now in the repo and folder name.

