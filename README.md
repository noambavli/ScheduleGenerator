
schedule generator
by noam bavli

**Before anyone asks, I know about "using namespace std", but I prefer not to use it because in large-scale projects (not like this, 
  real ones, i just do it all the time cause i think its good practice) 
  that require a lot of libraries,I’d rather know if a function comes from the standard library or another source. This helps avoid 
  conflicts with namespaces.
  Of course, I don’t mind using it, but I think it’s better to be explicit and not rely on using namespace std
**i have used the template print func even tho less efficient cause i think its pretty and the ui is basically for testing and in real 
  server-clinet environment i'ts not gonna print anything anyway. 

This is a samrt and cool schedule generator, specifically designed for schools and universities. 
The user provides teacher-class pairs, the number of days, and the number of hours (and ofc you can specify block times), and more preferences.
the generator creates an smart schedule.

It supports simultaneous scheduling for multiple classes, with a capacity for up to 99 teachers and no limit on the number of classes.

Key features include:

Teacher prioritization: Teacher prioritization for each teacher, for each class- For instance, in a sports class,you can prioritizes scheduling the sports teacher’s hours first.
(you can choose 1 - low/2 - mid/ 3- high level )

Block size prioritization :the generator prioritizes the chunks you choose for
each teacher, for each class- For example, if a math teacher for class x prefers four-hour blocks,the generator prioritizes that preference.

Logical optimizer for incomplete chunks: A logical optimizer for fixing incomplete chunks. For example, if a teacher is scheduled
for a chunk and it's interrupted by a blocked hour for that specific teacher, resulting in a replacement by another teacher,
the optimizer can resolve this by replacing the hours between surrounding teachers and finding suitable hours within the whole schedule for the replacements.

Customizable block times: You can set specific block times for individual classes or teachers, or apply them universally across all classes and teachers.

Note: The UI is currently horrible. More testing and user input validation are required.
The primary goal of this project is to implement all the algorithms and develop a system that processes data and generates accurate schedules.
During development, I used predefined data within the code. When you're ready to use the UI, simply comment out the predefined data 
section and uncomment the UI portion.

The schedule itself is stored as a 3D array. Other data is held in pairs, lists, and vectors, which might introduce some inconsistency.
I aimed to use the best structure for each case, though it's a bit confusing and may need further refinement.

The 3D array stores three-digit numbers: the hundreds digit represents the teacher, while the tens and ones digits represent the preferred time block.
Day-hour identifiers are stored as day*24 + hour, using basic mathematical operations for extremely fast data extraction, compression, and comparison.
The patterns for storing and extracting values are simple and do not compromise reliability.

**I don’t know why I thought until the last commits that “schedule” was spelled “shcedual.” I fixed it in the last commits in the files and now in the repo and folder name.

