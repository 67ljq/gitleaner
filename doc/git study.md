

# git study



[TOC]

​		全部是转载整理其他博主的，非本人原创。主要用来进一步理解版本和分支的概念，和记笔记，每次记不住命令都要上网查，还都零零碎碎的。

## 一、基础概念

### 1.2、时光机穿梭

​		我们已经成功地添加并提交了一个readme.txt文件，现在，是时候继续工作了，于是，我们继续修改readme.txt文件，改成如下内容：

```
Git is a distributed version control system.
Git is free software.
```

​		现在，运行`git status`命令看看结果：

```
$ git status
On branch master
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   readme.txt

no changes added to commit (use "git add" and/or "git commit -a")
```

​		`git status`命令可以让我们时刻掌握仓库当前的状态，上面的命令输出告诉我们，`readme.txt`被修改过了，但还没有准备提交的修改。

​		虽然Git告诉我们`readme.txt`被修改了，但如果能看看具体修改了什么内容，自然是很好的。比如你休假两周从国外回来，第一天上班时，已经记不清上次怎么修改的`readme.txt`，所以，需要用`git diff`这个命令看看：

```
$ git diff readme.txt 
diff --git a/readme.txt b/readme.txt
index 46d49bf..9247db6 100644
--- a/readme.txt
+++ b/readme.txt
@@ -1,2 +1,2 @@
-Git is a version control system.
+Git is a distributed version control system.
 Git is free software.
```

​		`git diff`顾名思义就是查看difference，显示的格式正是Unix通用的diff格式，可以从上面的命令输出看到，我们在第一行添加了一个`distributed`单词。

知道了对`readme.txt`作了什么修改后，再把它提交到仓库就放心多了，提交修改和提交新文件是一样的两步，第一步是`git add`：

```
$ git add readme.txt
```

​		同样没有任何输出。在执行第二步`git commit`之前，我们再运行`git status`看看当前仓库的状态：

```
$ git status
On branch master
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	modified:   readme.txt
```

​		`git status`告诉我们，将要被提交的修改包括`readme.txt`，下一步，就可以放心地提交了：

```
$ git commit -m "add distributed"
[master e475afc] add distributed
 1 file changed, 1 insertion(+), 1 deletion(-)
```

​		提交后，我们再用`git status`命令看看仓库的当前状态：

```
$ git status
On branch master
nothing to commit, working tree clean
```

​		Git告诉我们当前没有需要提交的修改，而且，工作目录是干净（working tree clean）的。

#### 1.2.1、版本回退

​		现在，你已经学会了修改文件，然后把修改提交到Git版本库，现在，再练习一次，修改readme.txt文件如下：

```
Git is a distributed version control system.
Git is free software distributed under the GPL.
```

​		然后尝试提交：

```
$ git add readme.txt
$ git commit -m "append GPL"
[master 1094adb] append GPL
 1 file changed, 1 insertion(+), 1 deletion(-)
```

​		像这样，你不断对文件进行修改，然后不断提交修改到版本库里，就好比玩RPG游戏时，每通过一关就会自动把游戏状态存盘，如果某一关没过去，你还可以选择读取前一关的状态。有些时候，在打Boss之前，你会手动存盘，以便万一打Boss失败了，可以从最近的地方重新开始。Git也是一样，每当你觉得文件修改到一定程度的时候，就可以“保存一个快照”，这个快照在Git中被称为`commit`。一旦你把文件改乱了，或者误删了文件，还可以从最近的一个`commit`恢复，然后继续工作，而不是把几个月的工作成果全部丢失。

​		现在，我们回顾一下`readme.txt`文件一共有几个版本被提交到Git仓库里了：

​		版本1：wrote a readme file

```
Git is a version control system.
Git is free software.
```

​		版本2：add distributed

```
Git is a distributed version control system.
Git is free software.
```

​		版本3：append GPL

```
Git is a distributed version control system.
Git is free software distributed under the GPL.
```

​		当然了，在实际工作中，我们脑子里怎么可能记得一个几千行的文件每次都改了什么内容，不然要版本控制系统干什么。版本控制系统肯定有某个命令可以告诉我们历史记录，在Git中，我们用`git log`命令查看：

```
$ git log
commit 1094adb7b9b3807259d8cb349e7df1d4d6477073 (HEAD -> master)
Author: Michael Liao <askxuefeng@gmail.com>
Date:   Fri May 18 21:06:15 2018 +0800

    append GPL

commit e475afc93c209a690c39c13a46716e8fa000c366
Author: Michael Liao <askxuefeng@gmail.com>
Date:   Fri May 18 21:03:36 2018 +0800

    add distributed

commit eaadf4e385e865d25c48e7ca9c8395c3f7dfaef0
Author: Michael Liao <askxuefeng@gmail.com>
Date:   Fri May 18 20:59:18 2018 +0800

    wrote a readme file
```

​		`git log`命令显示从最近到最远的提交日志，我们可以看到3次提交，最近的一次是`append GPL`，上一次是`add distributed`，最早的一次是`wrote a readme file`。

​		如果嫌输出信息太多，看得眼花缭乱的，可以试试加上`--pretty=oneline`参数：

```
$ git log --pretty=oneline
1094adb7b9b3807259d8cb349e7df1d4d6477073 (HEAD -> master) append GPL
e475afc93c209a690c39c13a46716e8fa000c366 add distributed
eaadf4e385e865d25c48e7ca9c8395c3f7dfaef0 wrote a readme file
```

​		需要友情提示的是，你看到的一大串类似`1094adb...`的是`commit id`（版本号），和SVN不一样，Git的`commit id`不是1，2，3……递增的数字，而是一个SHA1计算出来的一个非常大的数字，用十六进制表示，而且你看到的`commit id`和我的肯定不一样，以你自己的为准。为什么`commit id`需要用这么一大串数字表示呢？因为Git是分布式的版本控制系统，后面我们还要研究多人在同一个版本库里工作，如果大家都用1，2，3……作为版本号，那肯定就冲突了。

​		每提交一个新版本，实际上Git就会把它们自动串成一条时间线。如果使用可视化工具查看Git历史，就可以更清楚地看到提交历史的时间线：

![git-log-timeline](https://www.liaoxuefeng.com/files/attachments/919019707114272/0)

​		好了，现在我们启动时光穿梭机，准备把`readme.txt`回退到上一个版本，也就是`add distributed`的那个版本，怎么做呢？

​		首先，Git必须知道当前版本是哪个版本，在Git中，用`HEAD`表示当前版本，也就是最新的提交`1094adb...`（注意我的提交ID和你的肯定不一样），上一个版本就是`HEAD^`，上上一个版本就是`HEAD^^`，当然往上100个版本写100个`^`比较容易数不过来，所以写成`HEAD~100`。

​		现在，我们要把当前版本`append GPL`回退到上一个版本`add distributed`，就可以使用`git reset`命令：

```
$ git reset --hard HEAD^
HEAD is now at e475afc add distributed
```

​		`--hard`参数有啥意义？这个后面再讲，现在你先放心使用。

​		看看`readme.txt`的内容是不是版本`add distributed`：

```
$ cat readme.txt
Git is a distributed version control system.
Git is free software.
```

​		果然被还原了。

​		还可以继续回退到上一个版本`wrote a readme file`，不过且慢，让我们用`git log`再看看现在版本库的状态：

```
$ git log
commit e475afc93c209a690c39c13a46716e8fa000c366 (HEAD -> master)
Author: Michael Liao <askxuefeng@gmail.com>
Date:   Fri May 18 21:03:36 2018 +0800

    add distributed

commit eaadf4e385e865d25c48e7ca9c8395c3f7dfaef0
Author: Michael Liao <askxuefeng@gmail.com>
Date:   Fri May 18 20:59:18 2018 +0800

    wrote a readme file
```

​		最新的那个版本`append GPL`已经看不到了！好比你从21世纪坐时光穿梭机来到了19世纪，想再回去已经回不去了，肿么办？

​		办法其实还是有的，只要上面的命令行窗口还没有被关掉，你就可以顺着往上找啊找啊，找到那个`append GPL`的`commit id`是`1094adb...`，于是就可以指定回到未来的某个版本：

```
$ git reset --hard 1094a
HEAD is now at 83b0afe append GPL
```

​		版本号没必要写全，前几位就可以了，Git会自动去找。当然也不能只写前一两位，因为Git可能会找到多个版本号，就无法确定是哪一个了。

​		再小心翼翼地看看`readme.txt`的内容：

```
$ cat readme.txt
Git is a distributed version control system.
Git is free software distributed under the GPL.
```

​		果然，我胡汉三又回来了。

​		Git的版本回退速度非常快，因为Git在内部有个指向当前版本的`HEAD`指针，当你回退版本的时候，Git仅仅是把HEAD从指向`append GPL`：

```ascii
┌────┐
│HEAD│
└────┘
   │
   └──> ○ append GPL
        │
        ○ add distributed
        │
        ○ wrote a readme file
```

​		改为指向`add distributed`：

```ascii
┌────┐
│HEAD│
└────┘
   │
   │    ○ append GPL
   │    │
   └──> ○ add distributed
        │
        ○ wrote a readme file
```

​		然后顺便把工作区的文件更新了。所以你让`HEAD`指向哪个版本号，你就把当前版本定位在哪。

​		现在，你回退到了某个版本，关掉了电脑，第二天早上就后悔了，想恢复到新版本怎么办？找不到新版本的`commit id`怎么办？

​		在Git中，总是有后悔药可以吃的。当你用`$ git reset --hard HEAD^`回退到`add distributed`版本时，再想恢复到`append GPL`，就必须找到`append GPL`的commit id。Git提供了一个命令`git reflog`用来记录你的每一次命令：

```
$ git reflog
e475afc HEAD@{1}: reset: moving to HEAD^
1094adb (HEAD -> master) HEAD@{2}: commit: append GPL
e475afc HEAD@{3}: commit: add distributed
eaadf4e HEAD@{4}: commit (initial): wrote a readme file
```

​		终于舒了口气，从输出可知，`append GPL`的commit id是`1094adb`，现在，你又可以乘坐时光机回到未来了。

- 小结

  现在总结一下：

- `HEAD`指向的版本就是当前版本，因此，Git允许我们在版本的历史之间穿梭，使用命令`git reset --hard commit_id`。

- 穿梭前，用`git log`可以查看提交历史，以便确定要回退到哪个版本。

- 要重返未来，用`git reflog`查看命令历史，以便确定要回到未来的哪个版本。

#### 1.2.2、工作区和暂存区

​		Git和其他版本控制系统如SVN的一个不同之处就是有暂存区的概念。

​		先来看名词解释。

**工作区（Working Directory）**

​		就是你在电脑里能看到的目录，比如我的`learngit`文件夹就是一个工作区：

![working-dir](https://www.liaoxuefeng.com/files/attachments/919021113952544/0)

**版本库（Repository）**

​		工作区有一个隐藏目录`.git`，这个不算工作区，而是Git的版本库。

​		Git的版本库里存了很多东西，其中最重要的就是称为stage（或者叫index）的暂存区，还有Git为我们自动创建的第一个分支`master`，以及指向`master`的一个指针叫`HEAD`。

![git-repo](https://www.liaoxuefeng.com/files/attachments/919020037470528/0)

​		分支和`HEAD`的概念我们以后再讲。

​		前面讲了我们把文件往Git版本库里添加的时候，是分两步执行的：

​		第一步是用`git add`把文件添加进去，实际上就是把文件修改添加到暂存区；

​		第二步是用`git commit`提交更改，实际上就是把暂存区的所有内容提交到当前分支。

​		因为我们创建Git版本库时，Git自动为我们创建了唯一一个`master`分支，所以，现在，`git commit`就是往`master`分支上提交更改。

​		你可以简单理解为，需要提交的文件修改通通放到暂存区，然后，一次性提交暂存区的所有修改。

​		俗话说，实践出真知。现在，我们再练习一遍，先对`readme.txt`做个修改，比如加上一行内容：

```
Git is a distributed version control system.
Git is free software distributed under the GPL.
Git has a mutable index called stage.
```

​		然后，在工作区新增一个`LICENSE`文本文件（内容随便写）。

​		先用`git status`查看一下状态：

```
$ git status
On branch master
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   readme.txt

Untracked files:
  (use "git add <file>..." to include in what will be committed)

	LICENSE

no changes added to commit (use "git add" and/or "git commit -a")
```

​		Git非常清楚地告诉我们，`readme.txt`被修改了，而`LICENSE`还从来没有被添加过，所以它的状态是`Untracked`。

​		现在，使用两次命令`git add`，把`readme.txt`和`LICENSE`都添加后，用`git status`再查看一下：

```
$ git status
On branch master
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	new file:   LICENSE
	modified:   readme.txt
```

​		现在，暂存区的状态就变成这样了：

![git-stage](https://www.liaoxuefeng.com/files/attachments/919020074026336/0)

​		所以，`git add`命令实际上就是把要提交的所有修改放到暂存区（Stage），然后，执行`git commit`就可以一次性把暂存区的所有修改提交到分支。

```
$ git commit -m "understand how stage works"
[master e43a48b] understand how stage works
 2 files changed, 2 insertions(+)
 create mode 100644 LICENSE
```

​		一旦提交后，如果你又没有对工作区做任何修改，那么工作区就是“干净”的：

```
$ git status
On branch master
nothing to commit, working tree clean
```

​		现在版本库变成了这样，暂存区就没有任何内容了：

![git-stage-after-commit](https://www.liaoxuefeng.com/files/attachments/919020100829536/0)

- ### 小结

  暂存区是Git非常重要的概念，弄明白了暂存区，就弄明白了Git的很多操作到底干了什么。

  没弄明白暂存区是怎么回事的童鞋，请向上滚动页面，再看一次。

#### 1.2.3、管理修改

​		现在，假定你已经完全掌握了暂存区的概念。下面，我们要讨论的就是，为什么Git比其他版本控制系统设计得优秀，因为Git跟踪并管理的是修改，而非文件。

​		你会问，什么是修改？比如你新增了一行，这就是一个修改，删除了一行，也是一个修改，更改了某些字符，也是一个修改，删了一些又加了一些，也是一个修改，甚至创建一个新文件，也算一个修改。

​		为什么说Git管理的是修改，而不是文件呢？我们还是做实验。第一步，对readme.txt做一个修改，比如加一行内容：

```
$ cat readme.txt
Git is a distributed version control system.
Git is free software distributed under the GPL.
Git has a mutable index called stage.
Git tracks changes.
```

​		然后，添加：

```
$ git add readme.txt
$ git status
# On branch master
# Changes to be committed:
#   (use "git reset HEAD <file>..." to unstage)
#
#       modified:   readme.txt
#
```

​		然后，再修改readme.txt：

```
$ cat readme.txt 
Git is a distributed version control system.
Git is free software distributed under the GPL.
Git has a mutable index called stage.
Git tracks changes of files.
```

​		提交：

```
$ git commit -m "git tracks changes"
[master 519219b] git tracks changes
 1 file changed, 1 insertion(+)
```

​		提交后，再看看状态：

```
$ git status
On branch master
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   readme.txt

no changes added to commit (use "git add" and/or "git commit -a")
```

​		咦，怎么第二次的修改没有被提交？

​		别激动，我们回顾一下操作过程：

​		第一次修改 -> `git add` -> 第二次修改 -> `git commit`

​		你看，我们前面讲了，Git管理的是修改，当你用`git add`命令后，在工作区的第一次修改被放入暂存区，准备提交，但是，在工作区的第二次修改并没有放入暂存区，所以，`git commit`只负责把暂存区的修改提交了，也就是第一次的修改被提交了，第二次的修改不会被提交。

​		提交后，用`git diff HEAD -- readme.txt`命令可以查看工作区和版本库里面最新版本的区别：

```
$ git diff HEAD -- readme.txt 
diff --git a/readme.txt b/readme.txt
index 76d770f..a9c5755 100644
--- a/readme.txt
+++ b/readme.txt
@@ -1,4 +1,4 @@
 Git is a distributed version control system.
 Git is free software distributed under the GPL.
 Git has a mutable index called stage.
-Git tracks changes.
+Git tracks changes of files.
```

​		可见，第二次修改确实没有被提交。

​		那怎么提交第二次修改呢？你可以继续`git add`再`git commit`，也可以别着急提交第一次修改，先`git add`第二次修改，再`git commit`，就相当于把两次修改合并后一块提交了：

​		第一次修改 -> `git add` -> 第二次修改 -> `git add` -> `git commit`

​		好，现在，把第二次修改提交了，然后开始小结。

- 小结

​		现在，你又理解了Git是如何跟踪修改的，每次修改，如果不用`git add`到暂存区，那就不会加入到`commit`中。

#### 1.2.4、撤销修改

​		自然，你是不会犯错的。不过现在是凌晨两点，你正在赶一份工作报告，你在`readme.txt`中添加了一行：

```
$ cat readme.txt
Git is a distributed version control system.
Git is free software distributed under the GPL.
Git has a mutable index called stage.
Git tracks changes of files.
My stupid boss still prefers SVN.
```

​		在你准备提交前，一杯咖啡起了作用，你猛然发现了`stupid boss`可能会让你丢掉这个月的奖金！

​		既然错误发现得很及时，就可以很容易地纠正它。你可以删掉最后一行，手动把文件恢复到上一个版本的状态。如果用`git status`查看一下：

```
$ git status
On branch master
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   readme.txt

no changes added to commit (use "git add" and/or "git commit -a")
```

​		你可以发现，Git会告诉你，`git checkout -- file`可以丢弃工作区的修改：

```
$ git checkout -- readme.txt
```

​		命令`git checkout -- readme.txt`意思就是，把`readme.txt`文件在工作区的修改全部撤销，这里有两种情况：

​		一种是`readme.txt`自修改后还没有被放到暂存区，现在，撤销修改就回到和版本库一模一样的状态；

​		一种是`readme.txt`已经添加到暂存区后，又作了修改，现在，撤销修改就回到添加到暂存区后的状态。

​		总之，就是让这个文件回到最近一次`git commit`或`git add`时的状态。

​		现在，看看`readme.txt`的文件内容：

```
$ cat readme.txt
Git is a distributed version control system.
Git is free software distributed under the GPL.
Git has a mutable index called stage.
Git tracks changes of files.
```

​		文件内容果然复原了。

​		`git checkout -- file`命令中的`--`很重要，没有`--`，就变成了“切换到另一个分支”的命令，我们在后面的分支管理中会再次遇到`git checkout`命令。

​		现在假定是凌晨3点，你不但写了一些胡话，还`git add`到暂存区了：

```
$ cat readme.txt
Git is a distributed version control system.
Git is free software distributed under the GPL.
Git has a mutable index called stage.
Git tracks changes of files.
My stupid boss still prefers SVN.

$ git add readme.txt
```

​		庆幸的是，在`commit`之前，你发现了这个问题。用`git status`查看一下，修改只是添加到了暂存区，还没有提交：

```
$ git status
On branch master
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	modified:   readme.txt
```

​		Git同样告诉我们，用命令`git reset HEAD <file>`可以把暂存区的修改撤销掉（unstage），重新放回工作区：

```
$ git reset HEAD readme.txt
Unstaged changes after reset:
M	readme.txt
```

​		`git reset`命令既可以回退版本，也可以把暂存区的修改回退到工作区。当我们用`HEAD`时，表示最新的版本。

​		再用`git status`查看一下，现在暂存区是干净的，工作区有修改：

```
$ git status
On branch master
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   readme.txt
```

​		还记得如何丢弃工作区的修改吗？

```
$ git checkout -- readme.txt

$ git status
On branch master
nothing to commit, working tree clean
```

​		整个世界终于清静了！

​		现在，假设你不但改错了东西，还从暂存区提交到了版本库，怎么办呢？还记得[版本回退](https://www.liaoxuefeng.com/wiki/896043488029600/897013573512192)一节吗？可以回退到上一个版本。不过，这是有条件的，就是你还没有把自己的本地版本库推送到远程。还记得Git是分布式版本控制系统吗？我们后面会讲到远程版本库，一旦你把`stupid boss`提交推送到远程版本库，你就真的惨了……

- 小结

  又到了小结时间。

  场景1：当你改乱了工作区某个文件的内容，想直接丢弃工作区的修改时，用命令`git checkout -- file`。

  场景2：当你不但改乱了工作区某个文件的内容，还添加到了暂存区时，想丢弃修改，分两步，第一步用命令`git reset HEAD <file>`，就回到了场景1，第二步按场景1操作。

  场景3：已经提交了不合适的修改到版本库时，想要撤销本次提交，参考[版本回退](https://www.liaoxuefeng.com/wiki/896043488029600/897013573512192)一节，不过前提是没有推送到远程库。

#### 1.2.5、删除文件

​		在Git中，删除也是一个修改操作，我们实战一下，先添加一个新文件`test.txt`到Git并且提交：

```
$ git add test.txt

$ git commit -m "add test.txt"
[master b84166e] add test.txt
 1 file changed, 1 insertion(+)
 create mode 100644 test.txt
```

​		一般情况下，你通常直接在文件管理器中把没用的文件删了，或者用`rm`命令删了：

```
$ rm test.txt
```

​		这个时候，Git知道你删除了文件，因此，工作区和版本库就不一致了，`git status`命令会立刻告诉你哪些文件被删除了：

```
$ git status
On branch master
Changes not staged for commit:
  (use "git add/rm <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	deleted:    test.txt

no changes added to commit (use "git add" and/or "git commit -a")
```

​		现在你有两个选择，一是确实要从版本库中删除该文件，那就用命令`git rm`删掉，并且`git commit`：

```
$ git rm test.txt
rm 'test.txt'

$ git commit -m "remove test.txt"
[master d46f35e] remove test.txt
 1 file changed, 1 deletion(-)
 delete mode 100644 test.txt
```

​		现在，文件就从版本库中被删除了。

​		另一种情况是删错了，因为版本库里还有呢，所以可以很轻松地把误删的文件恢复到最新版本：

```
$ git checkout -- test.txt
```

​		`git checkout`其实是用版本库里的版本替换工作区的版本，无论工作区是修改还是删除，都可以“一键还原”。

​		 注意：从来没有被添加到版本库就被删除的文件，是无法恢复的！

- 小结

  ​	命令`git rm`用于删除一个文件。如果一个文件已经被提交到版本库，那么你永远不用担心误删，但是要小心，你只能恢复文件到最新版本，你会丢失**最近一次提交后你修改的内容**。

### 1.3、分支

​		分支就是科幻电影里面的平行宇宙，当你正在电脑前努力学习Git的时候，另一个你正在另一个平行宇宙里努力学习SVN。

如果两个平行宇宙互不干扰，那对现在的你也没啥影响。不过，在某个时间点，两个平行宇宙合并了，结果，你既学会了Git又学会了SVN！

![learn-branches](https://www.liaoxuefeng.com/files/attachments/919021987875136/0)

​		分支在实际中有什么用呢？假设你准备开发一个新功能，但是需要两周才能完成，第一周你写了50%的代码，如果立刻提交，由于代码还没写完，不完整的代码库会导致别人不能干活了。如果等代码全部写完再一次提交，又存在丢失每天进度的巨大风险。

​		现在有了分支，就不用怕了。你创建了一个属于你自己的分支，别人看不到，还继续在原来的分支上正常工作，而你在自己的分支上干活，想提交就提交，直到开发完毕后，再一次性合并到原来的分支上，这样，既安全，又不影响别人工作。

​		其他版本控制系统如SVN等都有分支管理，但是用过之后你会发现，这些版本控制系统创建和切换分支比蜗牛还慢，简直让人无法忍受，结果分支功能成了摆设，大家都不去用。

​		但Git的分支是与众不同的，无论创建、切换和删除分支，Git在1秒钟之内就能完成！无论你的版本库是1个文件还是1万个文件。

#### 1.31、创建与合并分支

​		在[版本回退](https://www.liaoxuefeng.com/wiki/896043488029600/897013573512192)里，你已经知道，每次提交，Git都把它们串成一条时间线，这条时间线就是一个分支。截止到目前，只有一条时间线，在Git里，这个分支叫主分支，即`master`分支。`HEAD`严格来说不是指向提交，而是指向`master`，`master`才是指向提交的，所以，`HEAD`指向的就是当前分支。

​		一开始的时候，`master`分支是一条线，Git用`master`指向最新的提交，再用`HEAD`指向`master`，就能确定当前分支，以及当前分支的提交点：

![git-br-initial](https://www.liaoxuefeng.com/files/attachments/919022325462368/0)

​		每次提交，`master`分支都会向前移动一步，这样，随着你不断提交，`master`分支的线也越来越长。

当我们创建新的分支，例如`dev`时，Git新建了一个指针叫`dev`，指向`master`相同的提交，再把`HEAD`指向`dev`，就表示当前分支在`dev`上：

![git-br-create](https://www.liaoxuefeng.com/files/attachments/919022363210080/l)

​		你看，Git创建一个分支很快，因为除了增加一个`dev`指针，改改`HEAD`的指向，工作区的文件都没有任何变化！

​		不过，从现在开始，对工作区的修改和提交就是针对`dev`分支了，比如新提交一次后，`dev`指针往前移动一步，而`master`指针不变：

![git-br-dev-fd](https://www.liaoxuefeng.com/files/attachments/919022387118368/l)

​		假如我们在`dev`上的工作完成了，就可以把`dev`合并到`master`上。Git怎么合并呢？最简单的方法，就是直接把`master`指向`dev`的当前提交，就完成了合并：

![git-br-ff-merge](https://www.liaoxuefeng.com/files/attachments/919022412005504/0)

​		所以Git合并分支也很快！就改改指针，工作区内容也不变！

​		合并完分支后，甚至可以删除`dev`分支。删除`dev`分支就是把`dev`指针给删掉，删掉后，我们就剩下了一条`master`分支：

![git-br-rm](https://www.liaoxuefeng.com/files/attachments/919022479428512/0)

​		真是太神奇了，你看得出来有些提交是通过分支完成的吗？

​		下面开始实战。

​		首先，我们创建`dev`分支，然后切换到`dev`分支：

```
git checkout -b dev
```

​		`git checkout`命令加上`-b`参数表示创建并切换，相当于以下两条命令：

```
git branch dev
git checkout dev
```

​		然后，用`git branch`命令查看当前分支：

​		`git branch`命令会列出所有分支，当前分支前面会标一个`*`号。然后，我们就可以在`dev`分支上正常提交，比如对`readme.txt`做个修改，加上一行：

- ### switch

  我们注意到切换分支使用`git checkout <branch>`，而前面讲过的撤销修改则是`git checkout -- <file>`，同一个命令，有两种作用，确实有点令人迷惑。

  实际上，切换分支这个动作，用`switch`更科学。因此，最新版本的Git提供了新的`git switch`命令来切换分支：

  创建并切换到新的`dev`分支，可以使用：

  ```
  $ git switch -c dev
  ```

  直接切换到已有的`master`分支，可以使用：

  ```
  $ git switch master
  ```

  使用新的`git switch`命令，比`git checkout`要更容易理解。

- ### 小结

  Git鼓励大量使用分支：

  查看分支：`git branch`

  创建分支：`git branch <name>`

  切换分支：`git checkout <name>`或者`git switch <name>`

  创建+切换分支：`git checkout -b <name>`或者`git switch -c <name>`

  合并某分支到当前分支：`git merge <name>`

  删除分支：`git branch -d <name>`

#### 1.32、解决冲突

​		人生不如意之事十之八九，合并分支往往也不是一帆风顺的。

​		准备新的`feature1`分支，继续我们的新分支开发：

```
git switch -c feature1
```

​		修改`readme.txt`最后一行，改为：

```
Creating a new branch is quick AND simple.
```

​		在`feature1`分支上提交：

```
git add readme.txt
git commit -m "AND simple"
```

​		切换到`master`分支：

```
git switch master
```

​		在`master`分支上把`readme.txt`文件的最后一行改为：

```
Creating a new branch is quick & simple.
```

​		提交：

```
git add readme.txt 
git commit -m "& simple"
```

​		现在，`master`分支和`feature1`分支各自都分别有新的提交，变成了这样：

![git-br-feature1](https://www.liaoxuefeng.com/files/attachments/919023000423040/0)

​		这种情况下，Git无法执行“快速合并”，只能试图把各自的修改合并起来，但这种合并就可能会有冲突，我们试试看：

```
git merge feature1
```

​		果然冲突了！Git告诉我们，`readme.txt`文件存在冲突，必须手动解决冲突后再提交。`git status`也可以告诉我们冲突的文件：

```
$ git status
On branch master
Your branch is ahead of 'origin/master' by 2 commits.
  (use "git push" to publish your local commits)

You have unmerged paths.
  (fix conflicts and run "git commit")
  (use "git merge --abort" to abort the merge)

Unmerged paths:
  (use "git add <file>..." to mark resolution)

	both modified:   readme.txt

no changes added to commit (use "git add" and/or "git commit -a")
```

​		我们可以直接查看readme.txt的内容：

```
Git is a distributed version control system.
Git is free software distributed under the GPL.
Git has a mutable index called stage.
Git tracks changes of files.
<<<<<<< HEAD
Creating a new branch is quick & simple.
=======
Creating a new branch is quick AND simple.
>>>>>>> feature1
```

​		Git用`<<<<<<<`，`=======`，`>>>>>>>`标记出不同分支的内容，我们修改如下后保存：

```
Creating a new branch is quick and simple.
```

​		再提交：

```
$ git add readme.txt 
$ git commit -m "conflict fixed"
[master cf810e4] conflict fixed
```

​		现在，`master`分支和`feature1`分支变成了下图所示：

![git-br-conflict-merged](https://www.liaoxuefeng.com/files/attachments/919023031831104/0)

​		用带参数的`git log`也可以看到分支的合并情况：

```
$ git log --graph --pretty=oneline --abbrev-commit
*   cf810e4 (HEAD -> master) conflict fixed
|\  
| * 14096d0 (feature1) AND simple
* | 5dc6824 & simple
|/  
* b17d20e branch test
* d46f35e (origin/master) remove test.txt
* b84166e add test.txt
* 519219b git tracks changes
* e43a48b understand how stage works
* 1094adb append GPL
* e475afc add distributed
* eaadf4e wrote a readme file
```

​		最后，删除`feature1`分支：

```
git branch -d feature1
```

​		工作完成。

- ### 小结

  ​	当Git无法自动合并分支时，就必须首先解决冲突。解决冲突后，再提交，合并完成。

  ​	解决冲突就是把Git合并失败的文件手动编辑为我们希望的内容，再提交。

  ​	用`git log --graph`命令可以看到分支合并图。

#### 1.3.3、分支管理策略

​		通常，合并分支时，如果可能，Git会用`Fast forward`模式，但这种模式下，删除分支后，会丢掉分支信息。

​		如果要强制禁用`Fast forward`模式，Git就会在merge时生成一个新的commit，这样，从分支历史上就可以看出分支信息。

​		下面我们实战一下`--no-ff`方式的`git merge`：

​		首先，仍然创建并切换`dev`分支：

```
git switch -c dev
```

​		修改readme.txt文件，并提交一个新的commit：

```
git add readme.txt
git commit -m "add merge"
```

​		现在，我们切换回`master`：

```
git switch master
```

​		准备合并`dev`分支，请注意`--no-ff`参数，表示禁用`Fast forward`：

```
git merge --no-ff -m "merge with no-ff" dev
```

​		因为本次合并要创建一个新的commit，所以加上`-m`参数，把commit描述写进去。

​		合并后，我们用`git log`看看分支历史：

```
$ git log --graph --pretty=oneline --abbrev-commit
*   e1e9c68 (HEAD -> master) merge with no-ff
|\  
| * f52c633 (dev) add merge
|/  
*   cf810e4 conflict fixed
...
```

​		可以看到，不使用`Fast forward`模式，merge后就像这样：

![git-no-ff-mode](https://www.liaoxuefeng.com/files/attachments/919023225142304/0)

**分支策略**

​		在实际开发中，我们应该按照几个基本原则进行分支管理：

​		首先，`master`分支应该是非常稳定的，也就是仅用来发布新版本，平时不能在上面干活；

​		那在哪干活呢？干活都在`dev`分支上，也就是说，`dev`分支是不稳定的，到某个时候，比如1.0版本发布时，再把`dev`分支合并到`master`上，在`master`分支发布1.0版本；

​		你和你的小伙伴们每个人都在`dev`分支上干活，每个人都有自己的分支，时不时地往`dev`分支上合并就可以了。

​		所以，团队合作的分支看起来就像这样：

![git-br-policy](https://www.liaoxuefeng.com/files/attachments/919023260793600/0)

- ### 小结

  Git分支十分强大，在团队开发中应该充分应用。

  合并分支时，加上`--no-ff`参数就可以用普通模式合并，合并后的历史有分支，能看出来曾经做过合并，而`fast forward`合并就看不出来曾经做过合并。

#### 1.3.4、Bug分支

​		软件开发中，bug就像家常便饭一样。有了bug就需要修复，在Git中，由于分支是如此的强大，所以，每个bug都可以通过一个新的临时分支来修复，修复后，合并分支，然后将临时分支删除。

​		当你接到一个修复一个代号101的bug的任务时，很自然地，你想创建一个分支`issue-101`来修复它，但是，等等，当前正在`dev`上进行的工作还没有提交：

```
$ git status
On branch dev
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	new file:   hello.py

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   readme.txt
```

​		并不是你不想提交，而是工作只进行到一半，还没法提交，预计完成还需1天时间。但是，必须在两个小时内修复该bug，怎么办？

​		幸好，Git还提供了一个`stash`功能，可以把当前工作现场“储藏”起来，等以后恢复现场后继续工作：

```
$ git stash
Saved working directory and index state WIP on dev: f52c633 add merge
```

​		现在，用`git status`查看工作区，就是干净的（除非有没有被Git管理的文件），因此可以放心地创建分支来修复bug。

​		首先确定要在哪个分支上修复bug，假定需要在`master`分支上修复，就从`master`创建临时分支：

```
$ git checkout master
Switched to branch 'master'
Your branch is ahead of 'origin/master' by 6 commits.
  (use "git push" to publish your local commits)

$ git checkout -b issue-101
Switched to a new branch 'issue-101'
```

​		现在修复bug，需要把“Git is free software ...”改为“Git is a free software ...”，然后提交：

```
$ git add readme.txt 
$ git commit -m "fix bug 101"
[issue-101 4c805e2] fix bug 101
 1 file changed, 1 insertion(+), 1 deletion(-)
```

​		修复完成后，切换到`master`分支，并完成合并，最后删除`issue-101`分支：

```
$ git switch master
Switched to branch 'master'
Your branch is ahead of 'origin/master' by 6 commits.
  (use "git push" to publish your local commits)

$ git merge --no-ff -m "merged bug fix 101" issue-101
Merge made by the 'recursive' strategy.
 readme.txt | 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)
```

​		太棒了，原计划两个小时的bug修复只花了5分钟！现在，是时候接着回到`dev`分支干活了！

```
$ git switch dev
Switched to branch 'dev'

$ git status
On branch dev
nothing to commit, working tree clean
```

​		工作区是干净的，刚才的工作现场存到哪去了？用`git stash list`命令看看：

```
$ git stash list
stash@{0}: WIP on dev: f52c633 add merge
```

​		工作现场还在，Git把stash内容存在某个地方了，但是需要恢复一下，有两个办法：

​		一是用`git stash apply`恢复，但是恢复后，stash内容并不删除，你需要用`git stash drop`来删除；

​		另一种方式是用`git stash pop`，恢复的同时把stash内容也删了：

```
$ git stash pop
On branch dev
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	new file:   hello.py

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   readme.txt

Dropped refs/stash@{0} (5d677e2ee266f39ea296182fb2354265b91b3b2a)
```

​		再用`git stash list`查看，就看不到任何stash内容了：

```
$ git stash list
```

​		你可以多次stash，恢复的时候，先用`git stash list`查看，然后恢复指定的stash，用命令：

```
$ git stash apply stash@{0}
```

​		在master分支上修复了bug后，我们要想一想，dev分支是早期从master分支分出来的，所以，这个bug其实在当前dev分支上也存在。

​		那怎么在dev分支上修复同样的bug？重复操作一次，提交不就行了？

​		有木有更简单的方法？

​		有！

​		同样的bug，要在dev上修复，我们只需要把`4c805e2 fix bug 101`这个提交所做的修改“复制”到dev分支。注意：我们只想复制`4c805e2 fix bug 101`这个提交所做的修改，并不是把整个master分支merge过来。

​		为了方便操作，Git专门提供了一个`cherry-pick`命令，让我们能复制一个特定的提交到当前分支：

```
$ git branch
* dev
  master
$ git cherry-pick 4c805e2
[master 1d4b803] fix bug 101
 1 file changed, 1 insertion(+), 1 deletion(-)
```

​		Git自动给dev分支做了一次提交，注意这次提交的commit是`1d4b803`，它并不同于master的`4c805e2`，因为这两个commit只是改动相同，但确实是两个不同的commit。用`git cherry-pick`，我们就不需要在dev分支上手动再把修bug的过程重复一遍。

​		有些聪明的童鞋会想了，既然可以在master分支上修复bug后，在dev分支上可以“重放”这个修复过程，那么直接在dev分支上修复bug，然后在master分支上“重放”行不行？当然可以，不过你仍然需要`git stash`命令保存现场，才能从dev分支切换到master分支。

- ### 小结

  修复bug时，我们会通过创建新的bug分支进行修复，然后合并，最后删除；

  当手头工作没有完成时，先把工作现场`git stash`一下，然后去修复bug，修复后，再`git stash pop`，回到工作现场；

  在master分支上修复的bug，想要合并到当前dev分支，可以用`git cherry-pick <commit>`命令，把bug提交的修改“复制”到当前分支，避免重复劳动。

#### 1.3.5、Feature分支

​		软件开发中，总有无穷无尽的新的功能要不断添加进来。

​		添加一个新功能时，你肯定不希望因为一些实验性质的代码，把主分支搞乱了，所以，每添加一个新功能，最好新建一个feature分支，在上面开发，完成后，合并，最后，删除该feature分支。

​		现在，你终于接到了一个新任务：开发代号为Vulcan的新功能，该功能计划用于下一代星际飞船。

​		于是准备开发：

```
git switch -c feature-vulcan
```

​		5分钟后，开发完毕：

```
$ git add vulcan.py

$ git status
On branch feature-vulcan
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	new file:   vulcan.py

$ git commit -m "add feature vulcan"
[feature-vulcan 287773e] add feature vulcan
 1 file changed, 2 insertions(+)
 create mode 100644 vulcan.py
```

​		切回`dev`，准备合并：

```
git switch dev
```

​		一切顺利的话，feature分支和bug分支是类似的，合并，然后删除。

​		但是！

​		就在此时，接到上级命令，因经费不足，新功能必须取消！

​		虽然白干了，但是这个包含机密资料的分支还是必须就地销毁：

```
$ git branch -d feature-vulcan
error: The branch 'feature-vulcan' is not fully merged.
If you are sure you want to delete it, run 'git branch -D feature-vulcan'.
```

​		销毁失败。Git友情提醒，`feature-vulcan`分支还没有被合并，如果删除，将丢失掉修改，如果要强行删除，需要使用大写的`-D`参数。。

​		现在我们强行删除：

```
$ git branch -D feature-vulcan
Deleted branch feature-vulcan (was 287773e).
```

​		终于删除成功！

- ### 小结

  开发一个新feature，最好新建一个分支；

  如果要丢弃一个没有被合并过的分支，可以通过`git branch -D <name>`强行删除。

#### 1.3.6、多人协作

​		当你从远程仓库克隆时，实际上Git自动把本地的`master`分支和远程的`master`分支对应起来了，并且，远程仓库的默认名称是`origin`。

要查看远程库的信息，用`git remote`：

```
$ git remote
origin
```

​		或者，用`git remote -v`显示更详细的信息：

```
$ git remote -v
origin  git@github.com:michaelliao/learngit.git (fetch)
origin  git@github.com:michaelliao/learngit.git (push)
```

​		上面显示了可以抓取和推送的`origin`的地址。如果没有推送权限，就看不到push的地址。

- **推送分支**

​		推送分支，就是把该分支上的所有本地提交推送到远程库。推送时，要指定本地分支，这样，Git就会把该分支推送到远程库对应的远程分支上：

```
git push origin master
```

​		如果要推送其他分支，比如`dev`，就改成：

```
git push origin dev
```

​		但是，并不是一定要把本地分支往远程推送，那么，哪些分支需要推送，哪些不需要呢？

- `master`分支是主分支，因此要时刻与远程同步；
- `dev`分支是开发分支，团队所有成员都需要在上面工作，所以也需要与远程同步；
- bug分支只用于在本地修复bug，就没必要推到远程了，除非老板要看看你每周到底修复了几个bug；
- feature分支是否推到远程，取决于你是否和你的小伙伴合作在上面开发。

总之，就是在Git中，分支完全可以在本地自己藏着玩，是否推送，视你的心情而定！

- **抓取分支**

​		多人协作时，大家都会往`master`和`dev`分支上推送各自的修改。

​		现在，模拟一个你的小伙伴，可以在另一台电脑（注意要把SSH Key添加到GitHub）或者同一台电脑的另一个目录下克隆：

```
$ git clone git@github.com:michaelliao/learngit.git
Cloning into 'learngit'...
remote: Counting objects: 40, done.
remote: Compressing objects: 100% (21/21), done.
remote: Total 40 (delta 14), reused 40 (delta 14), pack-reused 0
Receiving objects: 100% (40/40), done.
Resolving deltas: 100% (14/14), done.
```

​		当你的小伙伴从远程库clone时，默认情况下，你的小伙伴只能看到本地的`master`分支。不信可以用`git branch`命令看看：

```
$ git branch
* master
```

​		现在，你的小伙伴要在`dev`分支上开发，就必须创建远程`origin`的`dev`分支到本地，于是他用这个命令创建本地`dev`分支：

```
git checkout -b dev origin/dev
```

​		现在，他就可以在`dev`上继续修改，然后，时不时地把`dev`分支`push`到远程：

```
$ git add env.txt

$ git commit -m "add env"
[dev 7a5e5dd] add env
 1 file changed, 1 insertion(+)
 create mode 100644 env.txt

$ git push origin dev
Counting objects: 3, done.
Delta compression using up to 4 threads.
Compressing objects: 100% (2/2), done.
Writing objects: 100% (3/3), 308 bytes | 308.00 KiB/s, done.
Total 3 (delta 0), reused 0 (delta 0)
To github.com:michaelliao/learngit.git
   f52c633..7a5e5dd  dev -> dev
```

​		你的小伙伴已经向`origin/dev`分支推送了他的提交，而碰巧你也对同样的文件作了修改，并试图推送：

```
$ cat env.txt
env

$ git add env.txt

$ git commit -m "add new env"
[dev 7bd91f1] add new env
 1 file changed, 1 insertion(+)
 create mode 100644 env.txt

$ git push origin dev
To github.com:michaelliao/learngit.git
 ! [rejected]        dev -> dev (non-fast-forward)
error: failed to push some refs to 'git@github.com:michaelliao/learngit.git'
hint: Updates were rejected because the tip of your current branch is behind
hint: its remote counterpart. Integrate the remote changes (e.g.
hint: 'git pull ...') before pushing again.
hint: See the 'Note about fast-forwards' in 'git push --help' for details.
```

​		推送失败，因为你的小伙伴的最新提交和你试图推送的提交有冲突，解决办法也很简单，Git已经提示我们，先用`git pull`把最新的提交从`origin/dev`抓下来，然后，在本地合并，解决冲突，再推送：

```
$ git pull
There is no tracking information for the current branch.
Please specify which branch you want to merge with.
See git-pull(1) for details.

    git pull <remote> <branch>

If you wish to set tracking information for this branch you can do so with:

    git branch --set-upstream-to=origin/<branch> dev
```

​		`git pull`也失败了，原因是没有指定本地`dev`分支与远程`origin/dev`分支的链接，根据提示，设置`dev`和`origin/dev`的链接：

```
$ git branch --set-upstream-to=origin/dev dev
Branch 'dev' set up to track remote branch 'dev' from 'origin'.
```

​		再pull：

```
$ git pull
Auto-merging env.txt
CONFLICT (add/add): Merge conflict in env.txt
Automatic merge failed; fix conflicts and then commit the result.
```

​		这回`git pull`成功，但是合并有冲突，需要手动解决，解决的方法和分支管理中的[解决冲突](http://www.liaoxuefeng.com/wiki/896043488029600/900004111093344)完全一样。解决后，提交，再push：

```
$ git commit -m "fix env conflict"
[dev 57c53ab] fix env conflict

$ git push origin dev
Counting objects: 6, done.
Delta compression using up to 4 threads.
Compressing objects: 100% (4/4), done.
Writing objects: 100% (6/6), 621 bytes | 621.00 KiB/s, done.
Total 6 (delta 0), reused 0 (delta 0)
To github.com:michaelliao/learngit.git
   7a5e5dd..57c53ab  dev -> dev
```

​		因此，多人协作的工作模式通常是这样：

1. 首先，可以试图用`git push origin <branch-name>`推送自己的修改；

2. 如果推送失败，则因为远程分支比你的本地更新，需要先用`git pull`试图合并；

3. 如果合并有冲突，则解决冲突，并在本地提交；

4. 没有冲突或者解决掉冲突后，再用`git push origin <branch-name>`推送就能成功！

   如果`git pull`提示`no tracking information`，则说明本地分支和远程分支的链接关系没有创建，用命令`git branch --set-upstream-to <branch-name> origin/<branch-name>`。

这就是多人协作的工作模式，一旦熟悉了，就非常简单。

- ### 小结

  - 查看远程库信息，使用`git remote -v`；
  - 本地新建的分支如果不推送到远程，对其他人就是不可见的；
  - 从本地推送分支，使用`git push origin branch-name`，如果推送失败，先用`git pull`抓取远程的新提交；
  - 在本地创建和远程分支对应的分支，使用`git checkout -b branch-name origin/branch-name`，本地和远程分支的名称最好一致；
  - 建立本地分支和远程分支的关联，使用`git branch --set-upstream branch-name origin/branch-name`；
  - 从远程抓取分支，使用`git pull`，如果有冲突，要先处理冲突。

### 1.4、标签

​		发布一个版本时，我们通常先在版本库中打一个标签（tag），这样，就唯一确定了打标签时刻的版本。将来无论什么时候，取某个标签的版本，就是把那个打标签的时刻的历史版本取出来。所以，标签也是版本库的一个快照。

​		Git的标签虽然是版本库的快照，但其实它就是指向某个commit的指针（跟分支很像对不对？但是分支可以移动，标签不能移动），所以，创建和删除标签都是瞬间完成的。

​		Git有commit，为什么还要引入tag？

​		“请把上周一的那个版本打包发布，commit号是6a5819e...”

​		“一串乱七八糟的数字不好找！”

​		如果换一个办法：

​		“请把上周一的那个版本打包发布，版本号是v1.2”

​		“好的，按照tag v1.2查找commit就行！”

​		所以，tag就是一个让人容易记住的有意义的名字，它跟某个commit绑在一起。

## 二、基本命令

### 2.1、仓库基本命令

​		`git init`命令把目录变成Git可以管理的仓库：

```
git init
```

​		`git add`告诉Git，把文件添加到仓库：

```
git add <*|xxx [xxx]...>
```

​		`git commit`告诉Git，把文件提交到仓库：

```
git commit [-m "注释"]
```

​		为什么Git添加文件需要`add`，`commit`一共两步呢？因为`commit`可以一次提交很多文件，所以你可以多次`add`不同的文件。

​		`git status`显示当前仓库状态

​		如果有修改会提示你有文件可以add，如果有add的文件会提示你有文件可以commit，还显示当前处于哪个分支。

​		`git diff [xxx]`会显示具体的xxx文件和最后commit文件的差异，**不建议这样用，直接用带gui的工具就好，直观清晰。**

### 2.2、查看提交历史记录

​		`git log`命令直接查看全部历史记录，但一般不这么干，一多起来后很乱。

### 2.3、版本回退

### 2.4、标签管理

1. 打标签

   在Git中打标签非常简单，首先，切换到需要打标签的分支上：`git tag <name> [commit id] `就可以打一个新标签，可以在指定的 commit id，默认是当前的head版本

```
git tag v1.0
```

2. 查看所有标签

```
git tag
```

3. 指定信息

   还可以提交打标签的说明，使用命令`git tag -a <tagname> -m "blablabla..."`可以附加记录打标签时的说明，`git show <tagname>`显示详细的信息。

4. 删除标签

   `git tag -d <tagname>`删除指定标签

5. 远端推送

   `git push origin <tagname> `推送某个标签到远端，

   `git push origin --tag`推送全部标签到远端

6. 远端删除

   如果标签已经推送到远程，要删除远程标签就麻烦一点，先从本地删除，然后，从远程删除。删除命令也是push，但是格式如下：

   ```
   git tag -d v0.9
   git push origin :refs/tags/v0.9
   ```

   

## 三、分支命令

### 3.1 从本地分支A创建分支B 并切换到B

​		在任意分支上执行

```
git checkout -b B  A
```

​		如果在A 分支上执行，那么上面语句的A可以省略

### 3.2、从远程分支A创建分支B 并切换到B

​		在任意分支上执行

```
git checkout -b B origin/A
```

### 3.3、从远端分支刷新本地分支commit 记录

​		git fetch <远程主机名>

```
git fetch
```

​		上面命令将某个远程主机的更新，全部刷新回本地。

```
git fetch origin master
```

​		将远程origin主机的master分支commit 刷新到本地。

​		默认情况下，刷新回来的数据在`.git/FETCH_HEAD`中

### 3.4、删除本地分支A

​		在非A的分支上执行

```
git branch -d A
```

### 3.5、强行删除本地分支A

​		在非A的分支上执行

```
git branch -D A
```

### 3.6、删除远程分支A

​		在任意分支上执行

```
git push origin --delete A
```

### 3.7、合并分支A 到分支 B上

​		在分支B分支上执行

``` 
git merge --no-ff A [-m 注释]
```

​		上面代码的–no-ff 表示不用快速合并,-m表示注释

### 3.8、上传代码到github

​		1、git remote add origin <仓库地址>

​		2、git pull origin master 同步远程仓库

​		3、add... commit...

​		4、git push -u origin master

### 3.9、电脑修改密码后，输入密码的提示不出现了，直接报 Authentication failed for

```
git config --system --unset credential.helper
```

​		或者

```
git config --global --unset credential.helper
```

### 3.10、结束一个git命令

​		输入Q键即可

### 3.11、创建远端分支

​		只要将本地分支push到远端就可以了

```
git push origin test:origin/test
```

​		上面的命令用本地分支test在远端创建了一个test分支

### 3.12、将本地分支与远端分支关联，如果远端分支不存在则创建并关联

```
git push --set-upstream origin branch_name
```

​		上面的命令将当前本地分支与名为branch_name的远端分支关联

### 3.13、取消本地分支与远端分支的关联关系

```
git push --unset-upstream origin branch_name
```

​		上面的命令将当前本地分支与名为branch_name的远端分支取消关联

### 3.14、从远端创建本地同名分支并关联

```
git checkout --track origin/branch_name 
```

​		从远端branch_name分支创建本地同名分支并关联

### 3.15、从远端创建本地自定义名称分支并关联

```
git checkout -b new_branch_name origin/branch_name
```

​		上面的命令以branch_name的远端分支创建本地分支new_branch_name

### 3.16、查看远端分支情况

```
git branch -r
```

### 3.17、从远端克隆某一个分支

​		git clone -b [remote repository address]

```
git clone https://github.com/shusheng007/learngit.git
```

​		上面的命令克隆我GitHub上的默认主分支：master

```
git clone -b newbranch https://github.com/shusheng007/learngit.git
```

​		上面的命令克隆我GitHub上的名为**newbranch** 的一个分支

### 3.18、打tag并提交远端

​		下面命令打的是 Annotated Tag，我们大部分情况应该打这种类型的tag

```
 git tag -a v1.0 -m "my version 1.0"
```

​		上面打了一个v1.0 版本的tag， -m 后面的是对此tag的描述

​		将tag推送到远端服务器

```
git push origin v1.0
```

## 四、管理项目的方式

​		在实际开发中如何使用`Git`没有一个标准答案，使用方式也是各式各样，很多基本上都是把Git当SVN来用。下面介绍的是一种经过实践的运行比较良好的管理方式。

### 主分支

​		实际开发中，一个仓库（通常只放一个项目）主要存在两条主分支：**master**与**develop**分支。这个两个分支的生命周期是整个项目周期。就是说，自创建出来就不会删除，会随着项目的不断开发不断的往里面添加代码。master分支是创建git仓库时自动生成的，随即我们就会从master分支创建develop分支，如下图所示。

![这里写图片描述](https://img-blog.csdn.net/20180624162549140?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NodVNoZW5nMDAwNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

- **master**：这个分支最为稳定，这个分支代表项目处于可发布的状态。

  例如王二狗向`master`分支合并了代码，那就意味着王二狗完成了此项目的一个待发布的版本，项目经理可以认为，此项目已经准备好发布新版本了。所以`master`分支不是随随便便就可以签入代码的地方，只有计划发布的版本功能在`develop`分支上全部完成，而且测试没有问题了才会合并到`master`上。

- **develop**：作为开发的分支，平行于master分支。

  例如王二狗要开发一个**注册功能**，那么他就会从`develop`分支上创建一个**feature**分支 `fb-register`（后面讲），在`fb-register`分支上将注册功能完成后，将代码合并到**develop**分支上。这个`fb-register`就完成了它的使命，可以删除了。项目经理看王二狗效率很高啊，于是：“二狗你顺带把登录功能也做了吧”。二狗心中暗暗骂道：日了个狗的，但是任务还的正常做，二狗就会重复上面的步骤：从develop分支上新创建一个名为`fb-login`的分支，喝杯咖啡继续开发，1个小时后登录功能写好了，二狗又会将这个分支的代码合并回**develop**分支后将其删除。



​		通过以上分析可以发现，我们可以使用Git hook 脚本自动发布发布新的版本，具体就是每当有代码从**develop**分支合并到**master**分支的时候，脚本就会自动触发，编译发布新的版本。

### 支持分支

​		这些分支都是为了程序员协同开发，以及应对项目的各种需求而存在的。这些分支都是为了解决某一个具体的问题而设立，当这个问题解决后，代码会合并回主分支**develop**或者**master**后删除，一般我们会人为分出三种分支。

- **Feature branches**：这种分支和我们程序员日常开发最为密切，称作功能分支。

  必须从**develop**分支创建，完成后合并回**develop**分支

  如下图所示。

  ![这里写图片描述](https://img-blog.csdn.net/20180624170234573?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NodVNoZW5nMDAwNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  具体事例可以参考上面王二狗完成登录注册功能时的做法。

- **Release branches**：这个分支用来分布新版本。

  从**develop**分支创建，完成后合并回**develop**与**master**分支。

  这个分支上可以做一些非常小的bug修复，当然，你也可以禁止在这个分支做任何bug的修复工作，而只做版本发布的相关操作，例如设置版本号等操作，那样的话那些发现的小bug就必须放到下一个版本修复了。如果在这个分支上发现了大bug，那么也绝对不能在这个分支上改，需要**Featrue**分支上改，走正常的流程。

  **实例**：王二狗开发完了登录注册功能后决定发一个版本V0.1，那么他先从**develop**分支上创建一个**Release** 分支`release-v0.1`，然后二狗在这个分支上把版本号等做了修改。正准备编译发布了，项目经理说：“二狗啊，你那个登录框好像向右偏移量**1个像素**，你可以调一下吗？”二狗心中有暗暗骂道：日了个狗，但是。。。你们懂得，功能还的正常改。不过二狗发现这个bug特别小，对项目其他部分不造成不可预知的问题，所以直接在release分支上改了，然后愉快的发布了版本1.0.版本上线后，二狗将这个分支分别合并回了**develop**与**master**分支，然后删除了这个分支。

- **Hotfix branches**：这个分支主要为修复线上特别紧急的bug准备的。

  必须从**master**分支创建，完成后合并回**develop**与**master**分支。

  如下图所示：

  ![img](https://img-blog.csdn.net/20180624172850247?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NodVNoZW5nMDAwNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  这个分支主要是解决线上版本的紧急bug修复的，例如突然版本V0.1上有一个致命bug，必须修复。那么我们就可以从**master** 分支上发布这个版本那个时间点 例如 **tag v0.1**（一般代码发布后会及时在master上打**tag**），来创建一个 `hotfix-v0.1.1`的分支，然后在这个分支上改bug，然后发布新的版本。最后将代码合并回**develop**与**master**分支。

  **实例**：某天夜里二狗正在和女朋友嘿咻呢，突然项目经理打来电话：“二狗啊，线上出了个大问题，大量用户无法登录，客服电话已经被打爆了，你紧急处理一下”。二狗心中默默骂道：日了个狗，然后就爬起来去改代码了。二狗先找到**master**分支上**tag v0.1** 的地方，然后新建了`hotfix-v0.1.1` 分支，默默的修bug去了。经过一个多小时的奋战，终于修复了，然后二狗改了版本号，发布了新版本。二狗将这个分支分别合并回了**develop**与**master**分支后删除了这个分支。终于搞定了，回头看看床上的女票已经进入了梦乡，二狗贼心不死，上前挑逗，此刻女票将满腹怨气集于一点，使出凌空一脚将其登于床下，随后甩一枕于二狗，二狗会意，趋客厅，抱枕卧于沙发。。。

### 总结图

​		上面的讲解最后汇成一张图

![这里写图片描述](https://img-blog.csdn.net/20180624174835949?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1NodVNoZW5nMDAwNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 五、一些深入

### 5.1、HEAD detached from xxx

​		Git 中的 HEAD 可以理解为一个指针，我们可以在命令行中输入 `cat .git/HEAD` 查看当前 HEAD 指向哪儿，一般它指向当前工作目录所在分支的最新提交。

```
cat .git/HEAD
ref: refs/heads/ // 正常
cad0be9ceb89f474c39360c4de337d4a8194cab0 // 游离状态
```

​		使用`git checkout` 来移动HEAD指针，移动的对象可以是分支指针也可以是快照。
​		HEAD指针可以指向快照也可以指向branch。当指向branch时提交后会和branch指针一起向后移动，当不指向branch提交时时则会在一个detached状态。

​		当使用 `git checkout < branch_name>` 切换分支时，HEAD 会移动到指定分支。

```
git checkout
```

​		但是如果使用的是 `git checkout `，即切换到指定的某一次提交，HEAD 就会处于 detached 状态（游离状态）
​		HEAD 游离状态的利与弊
​		利: 我们可以很方便地在历史版本之间互相切换，比如需要回到某次提交，直接 checkout 对应的 commit id 或者 tag 名即可。

​		弊：在这个基础上的提交会新开一个匿名分支且提交是无法可见保存的，一旦切到别的分支，游离状态以后的提交就不可追溯了。

```
git branch

(HEAD detached at 925fda6)
master
```

​		在游离状态无法提交修改
**解决办法**
​		1，查看当前分支状态

```
git branch

(HEAD detached at 925fda6)
master
```

​		2，新建一个临时 tem 分支，把当前提交的代码放到整个分支

```
git branch tem
git checkout tem
```

​		3，换回要回到的那个分支，这里是 master

```
git checkout master
```

​		4，然后 merge 刚才创建的临时分支

```
git merge tem
Updating cad0be9…2437c6b
Fast-forward
…
```

​		5，检查是否有冲突，没有就提交到远端

```
git push origin master
```

​		6，删除临时分支

```
git branch -d tem
```

### 5.2、Git Reset 三种模式

​		有时候，我们用Git的时候有可能commit提交代码后，发现这一次commit的内容是有错误的，那么有两种处理方法：
​		1、修改错误内容，再次commit一次	 2、使用**git reset** 命令撤销这一次错误的commit
​		第一种方法比较直接，但会多次一次commit记录。
 		而我个人更倾向第二种方法，错误的commit没必要保留下来。
​		 那么今天来说一下**git reset**。它的一句话概括

```undefined
git-reset - Reset current HEAD to the specified state
```

​		意思就是可以让HEAD这个指针指向其他的地方。例如我们有一次commit不是不是很满意，需要回到上一次的Commit里面。那么这个时候就需要通过reset，把HEAD指针指向上一次的commit的点。
​		它有三种模式，soft,mixed,hard，具体的使用方法下面这张图，展示的很全面了。

![img](https:////upload-images.jianshu.io/upload_images/4428238-fcad08ebe26933a6.png?imageMogr2/auto-orient/strip|imageView2/2/w/638/format/webp)

​		git各个区域和命令关系

​		这三个模式理解了，对于使用这个命令很有帮助。在理解这三个模式之前，需要略微知道一点Git的基本流程。正如上图，Git会有三个区域：

- **Working Tree** 当前的工作区域
- **Index/Stage** 暂存区域，和git stash命令暂存的地方不一样。使用git add xx，就可以将xx添加近Stage里面
- **Repository** 提交的历史，即使用git commit提交后的结果

![img](https:////upload-images.jianshu.io/upload_images/4428238-5b04868bd09cb72a.png?imageMogr2/auto-orient/strip|imageView2/2/w/546/format/webp)

​																											**文件存入Repository流程**

​		以下简单敘述一下把文件存入Repository流程：

0. 刚开始 working tree 、 index 与 repository(HEAD)里面的內容都是一致的

![img](https:////upload-images.jianshu.io/upload_images/4428238-fb3e9ca4dce0328c.png?imageMogr2/auto-orient/strip|imageView2/2/w/469/format/webp)

​																																	**阶段1**

1. 当git管理的文件夹里面的内容出现改变后，此時 working tree 的內容就会跟 index 及 repository(HEAD)的不一致，而Git知道是哪些文件(Tracked File)被改动过，直接将文件状态设置为 modified (Unstaged files)。

   ![img](https:////upload-images.jianshu.io/upload_images/4428238-e92ef69bc699fad5.png?imageMogr2/auto-orient/strip|imageView2/2/w/465/format/webp)

   ​																																**阶段2**

2. 当我們执行 git add 后，会将这些改变的文件內容加入 index 中 (Staged files)，所以此时working tree跟index的內容是一致的，但他们与repository(HEAD)內容不一致。

   ![img](https:////upload-images.jianshu.io/upload_images/4428238-0b04f397c336d245.png?imageMogr2/auto-orient/strip|imageView2/2/w/466/format/webp)

   ​																															**阶段3**

3. 接着执行 git commit 後，將Git索引中所有改变的文件內容提交至 Repository 中，建立出新的 commit 节点(HEAD)后， working tree 、 index 與与repository(HEAD)区域的内容 又会保持一致。

   ![img](https:////upload-images.jianshu.io/upload_images/4428238-75a651c0a39381a0.png?imageMogr2/auto-orient/strip|imageView2/2/w/473/format/webp)

   ​																																**阶段4**

#### 5.2.1、实战演示

**一、reset --hard：重置stage区和工作目录:**

​		**reset --hard** 会在重置 **HEAD** 和**branch**的同时，重置stage区和工作目录里的内容。当你在 **reset** 后面加了 **--hard** 参数时，你的stage区和工作目录里的内容会被完全重置为和**HEAD**的新位置相同的内容。换句话说，就是你的没有**commit**的修改会被全部擦掉。

​		例如你在上次 **commit** 之后又对文件做了一些改动：把修改后的**ganmes.txt**文件**add**到**stage区**，修改后的**shopping list.txt**保留在**工作目录**

```shell
git status
```

![img](https:////upload-images.jianshu.io/upload_images/4428238-1c22b16e14586320.png?imageMogr2/auto-orient/strip|imageView2/2/w/621/format/webp)

​																														**最初状态**
​		然后，你执行了**reset**并附上了**--hard**参数

```shell
git reset --hard HEAD^
```

​		你的 **HEAD \**和当前\** branch** 切到上一条**commit** 的同时，你工作目录里的新改动和已经add到stage区的新改动也一起全都消失了：

```shell
git status
```

![img](https:////upload-images.jianshu.io/upload_images/4428238-3e57cea5e5400c94.png?imageMogr2/auto-orient/strip|imageView2/2/w/355/format/webp)

​																											**reset --hard head^之后**


​		可以看到，在 **reset --hard** 后，所有的改动都被擦掉了。

**二、reset --soft：保留工作目录，并把重置 HEAD 所带来的新的差异放进暂存区**

​		**reset --soft** 会在重置 **HEAD** 和 **branch** 时，保留工作目录和暂存区中的内容，并把重置 **HEAD** 所带来的新的差异放进暂存区。

​		什么是「重置 **HEAD** 所带来的新的差异」？就是这里：

![img](https:////upload-images.jianshu.io/upload_images/4428238-75ef41dc9eec6f8e?imageMogr2/auto-orient/strip|imageView2/2/w/478/format/webp)

​		由于 **HEAD** 从 4 移动到了 3，而且在 reset 的过程中工作目录和暂存区的内容没有被清理掉，所以 4 中的改动在 **reset** 后就也成了工作目录新增的「工作目录和 **HEAD** 的差异」。这就是上面一段中所说的「重置 **HEAD** 所带来的差异」。

​		此模式下会保留 **working tree工作目录**的內容，不会改变到目前所有的git管理的文件夹的內容；也会保留 **index暂存区**的內容，让 **index 暂存区**与 **working tree** 工作目录的內容是一致的。就只有 **repository** 中的內容的更变需要与 **reset** 目标节点一致，因此原始节点与**reset**节点之间的差异变更集合会存在与index暂存区中(**Staged files**)，所以我们可以直接执行 **git commit** 將 **index暂存区**中的內容提交至 **repository** 中。当我们想合并「当前节点」与「reset目标节点」之间不具太大意义的 **commit** 记录(可能是阶段性地频繁提交)時，可以考虑使用 **Soft Reset** 来让 **commit** 演进线图较为清晰点。

![img](https:////upload-images.jianshu.io/upload_images/4428238-2fcf8c37b866d331.png?imageMogr2/auto-orient/strip|imageView2/2/w/422/format/webp)

​		所以在同样的情况下，还是老样子：把修改后的**ganmes.txt**文件**add**到**stage区**，修改后的**shopping list.txt**保留在**工作目录**

```shell
git status
```

![img](https:////upload-images.jianshu.io/upload_images/4428238-0e5563f9d547f694.png?imageMogr2/auto-orient/strip|imageView2/2/w/621/format/webp)

​																														**最初状态**
​		假设此时当前 **commit** 的改动内容是新增了 **laughters.txt** 文件：

```shell
git show --stat
```

![img](https:////upload-images.jianshu.io/upload_images/4428238-1ac751cca1b142da.png?imageMogr2/auto-orient/strip|imageView2/2/w/533/format/webp)

​																															**git show --stat**

​		如果这时你执行：

```shell
git reset --soft HEAD^
```

​		那么除了 **HEAD** 和它所指向的 **branch1** 被移动到 **HEAD^** 之外，原先 **HEAD** 处 **commit** 的改动（也就是那个 **laughters.txt** 文件）也会被放进暂存区：

```shell
git status
```

![img](https:////upload-images.jianshu.io/upload_images/4428238-65f172789e8446da.png?imageMogr2/auto-orient/strip|imageView2/2/w/626/format/webp)

​																											**使用git reset --soft HEAD^后**

​		这就是**--soft** 和 **--hard** 的区别：**--hard** 会清空工作目录和暂存区的改动,*而 **--soft则会保留工作目录的内容，并把因为保留工作目录内容所带来的新的文件差异放进暂存区**。

**三、reset 不加参数(mixed)：保留工作目录，并清空暂存区**

​		**reset** 如果不加参数，那么默认使用 **--mixed** 参数。它的行为是：保留工作目录，并且清空暂存区。也就是说，工作目录的修改、暂存区的内容以及由 **reset** 所导致的新的文件差异，都会被放进工作目录。简而言之，就是「把所有差异都混合（mixed）放在工作目录中」。

​		还以同样的情况为例：

```shell
git status
```

![img](https:////upload-images.jianshu.io/upload_images/4428238-5e1d7adf0b472564.png?imageMogr2/auto-orient/strip|imageView2/2/w/621/format/webp)

​																															**最初状态**

​		**修改了 的games.txt 和 shopping list.txt，并把 games.txt 放进了暂存区。**

```shell
git show --stat
```

![img](https:////upload-images.jianshu.io/upload_images/4428238-5ed83eef2b0c00e7.png?imageMogr2/auto-orient/strip|imageView2/2/w/533/format/webp)

​																															**git show --stat**

​		**最新的 commit 中新增了 laughters.txt 文件。**

​		这时如果你执行**无参数**的**reset**或者带**--mixed**参数：

```shell
git reset HEAD^
git reset --mixed HEAD^
```

​		工作目录的内容和 **--soft** 一样会被保留，但和 **--soft** 的区别在于，它会把暂存区清空,并把原节点和**reset**节点的差异的文件放在工作目录，总而言之就是，工作目录的修改、暂存区的内容以及由 **reset** 所导致的新的文件差异，都会被放进工作目录

```shell
git status
```

![img](https:////upload-images.jianshu.io/upload_images/4428238-64770040c28c4c97.png?imageMogr2/auto-orient/strip|imageView2/2/w/625/format/webp)

​																											**git reset HEAD^之后**

- **总结**

  *reset 的本质：移动 HEAD 以及它所指向的 branch**

  实质上，**reset** 这个指令虽然可以用来撤销 **commit** ，但它的实质行为并不是撤销，而是移动 **HEAD** ，并且「捎带」上 **HEAD** 所指向的 **branch**（如果有的话）。也就是说，**reset** 这个指令的行为其实和它的字面意思 "**reset**"（重置）十分相符：它是用来重置 **HEAD** 以及它所指向的 **branch** 的位置的。

  而 **reset --hard HEAD^** 之所以起到了撤销 **commit** 的效果，是因为它把 **HEAD** 和它所指向的 branch 一起移动到了当前 **commit** 的父 **commit** 上，从而起到了「撤销」的效果：

![img](https:////upload-images.jianshu.io/upload_images/4428238-6dbab74ae9ad2e1f?imageMogr2/auto-orient/strip|imageView2/2/w/466/format/webp)

​																															**git reset**

​		Git 的历史只能往回看，不能向未来看，所以把 **HEAD** 和 **branch** 往回移动，就能起到撤回 **commit** 的效果。

所以同理，**reset --hard** 不仅可以撤销提交，还可以用来把 **HEAD** 和 **branch** 移动到其他的任何地方。

```shell
git reset --hard branch2
```

![img](https:////upload-images.jianshu.io/upload_images/4428238-71f7141a3878da7e?imageMogr2/auto-orient/strip|imageView2/2/w/434/format/webp)

​																													**git reset --hard branch2**

#### 5.2.2、reset三种模式区别和使用场景

**区别：**

1. **--hard**：重置位置的同时，直接将 **working Tree工作目录**、 **index 暂存区**及 **repository** 都重置成目标**Reset**节点的內容,所以效果看起来等同于清空暂存区和工作区。
2. **--soft**：重置位置的同时，保留**working Tree工作目录**和**index暂存区**的内容，只让**repository**中的内容和 **reset** 目标节点保持一致，因此原节点和**reset**节点之间的【差异变更集】会放入**index暂存区**中(**Staged files**)。所以效果看起来就是工作目录的内容不变，暂存区原有的内容也不变，只是原节点和**Reset**节点之间的所有差异都会放到暂存区中。
3. **--mixed（默认）**：重置位置的同时，只保留**Working Tree工作目录**的內容，但会将 **Index暂存区** 和 **Repository** 中的內容更改和reset目标节点一致，因此原节点和**Reset**节点之间的【差异变更集】会放入**Working Tree工作目录**中。所以效果看起来就是原节点和**Reset**节点之间的所有差异都会放到工作目录中。

**使用场景:**

1. **--hard**：(1) **要放弃目前本地的所有改变時**，即去掉所有add到暂存区的文件和工作区的文件，可以执行 **git reset -hard HEAD** 来强制恢复git管理的文件夹的內容及状态；(2) **真的想抛弃目标节点后的所有commit**（可能觉得目标节点到原节点之间的commit提交都是错了，之前所有的commit有问题）。
2. **--soft**：原节点和**reset**节点之间的【差异变更集】会放入**index暂存区**中(**Staged files**)，所以假如我们之前工作目录没有改过任何文件，也没add到暂存区，那么使用**reset  --soft**后，我们可以直接执行 **git commit** 將 index暂存区中的內容提交至 **repository** 中。为什么要这样呢？这样做的使用场景是：假如我们想合并「当前节点」与「**reset**目标节点」之间不具太大意义的 **commit** 记录(可能是阶段性地频繁提交,就是开发一个功能的时候，改或者增加一个文件的时候就**commit**，这样做导致一个完整的功能可能会好多个**commit**点，这时假如你需要把这些**commit**整合成一个**commit**的时候)時，可以考虑使用**reset  --soft**来让 **commit** 演进线图较为清晰。总而言之，**可以使用--soft合并commit节点**。
3. **--mixed（默认）**：(1)使用完**reset --mixed**后，我們可以直接执行 **git add** 将這些改变果的文件內容加入 **index 暂存区**中，再执行 **git commit** 将 **Index暂存区** 中的內容提交至**Repository**中，这样一样可以达到合并**commit**节点的效果（与上面--soft合并commit节点差不多，只是多了git add添加到暂存区的操作）；(2)移除所有Index暂存区中准备要提交的文件(Staged files)，我们可以执行 **git reset HEAD** 来 **Unstage** 所有已列入 **Index暂存区** 的待提交的文件。(有时候发现add错文件到暂存区，就可以使用命令)。(3)**commit**提交某些错误代码，或者没有必要的文件也被**commit**上去，不想再修改错误再**commit**（因为会留下一个错误**commit**点），可以回退到正确的**commit**点上，然后所有原节点和**reset**节点之间差异会返回工作目录，假如有个没必要的文件的话就可以直接删除了，再**commit**上去就OK了。

### 5.3、git stash 用法总结和注意点

​		常用git stash命令：

（1）**git stash** save "save message" : 执行存储时，添加备注，方便查找，只有git stash 也要可以的，但查找时不方便识别。

（2）**git stash list** ：查看stash了哪些存储

（3）**git stash show** ：显示做了哪些改动，默认show第一个存储,如果要显示其他存贮，后面加stash@{$num}，比如第二个 git stash show stash@{1}

（4）**git stash show -p** : 显示第一个存储的改动，如果想显示其他存存储，命令：git stash show stash@{$num} -p ，比如第二个：git stash show stash@{1} -p

（5）**git stash apply** :应用某个存储,但不会把存储从存储列表中删除，默认使用第一个存储,即stash@{0}，如果要使用其他个，git stash apply stash@{$num} ， 比如第二个：git stash apply stash@{1} 

（6）**git stash pop** ：命令恢复之前缓存的工作目录，将缓存堆栈中的对应stash删除，并将对应修改应用到当前的工作目录下,默认为第一个stash,即stash@{0}，如果要应用并删除其他stash，命令：git stash pop stash@{$num} ，比如应用并删除第二个：git stash pop stash@{1}

（7）**git stash drop** stash@{$num} ：丢弃stash@{$num}存储，从列表中删除这个存储

（8）`**git stash clear** ：`删除所有缓存的stash

 

**说明:新增的文件，直接执行stash是不会被存储的**，举例如下：

![img](https://img2018.cnblogs.com/blog/1282849/201809/1282849-20180914114003801-439062377.png)

如上图：在git status 那一步很明显可以看出来，我修改了README，添加了新文件abc.txt，然后执行了git stash save后，在执行git stash list 可以看到刚才的save是的信息，然后使用git stash show ，只显示了README的改动被存起来了。

我们知道，执行了git statsh 以后，被存起来的在当前目录再执行git status 就看不到了，但是我们现在再执行git status，如下：

![img](https://img2018.cnblogs.com/blog/1282849/201809/1282849-20180914114508020-1990121956.png)

这个文件还在，说明没有被存起来。说白了就是没有在git 版本控制中的文件，是不能被git stash 存起来的。

那要怎么办呢，这个文件我也想存起来，很明显，先执行下git add 加到git版本控制中，然后再git stash就可以了，如下：

![img](https://img2018.cnblogs.com/blog/1282849/201809/1282849-20180914114910946-1730309669.png)

最后一步可以看出来，这个新增文件已经被stash了。

这个时候再执行下git status ,被存起来的在当前目录就看不到了,如下：

![img](https://img2018.cnblogs.com/blog/1282849/201809/1282849-20180914115815867-407418856.png)

这个时候，想切分支就再也不会报错有改动未提交了。

如果要应用这些stash，直接使用git stash apply或者git stash pop就可以再次导出来了。

总结下：git add 只是把文件加到git 版本控制里，并不等于就被stash起来了，git add和git stash 没有必然的关系，但是执行git stash 能正确存储的前提是文件必须在git 版本控制中才行。

参考的一个链接中说到了以下，我摘录此处备份下（就是只stash一部分文件）：

   常规 git stash 的一个限制是它会一下暂存所有的文件。有时，只备份某些文件更为方便，让另外一些与代码库保持一致。一个非常有用的技巧，用来备份部分文件：

1. add 那些你不想备份的文件（例如： git add file1.js, file2.js）
2. 调用 git stash –keep-index。只会备份那些没有被add的文件。
3. 调用 git reset 取消已经add的文件的备份，继续自己的工作。

## 六、来源参考

1. [实际项目中如何使用Git做分支管理](https://blog.csdn.net/ShuSheng0007/article/details/80791849?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.control&dist_request_id=2e82118d-cd9f-4ff7-bc4e-cd4ca98991e6&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.control)
2. [Git日常开发常用命令汇总](https://blog.csdn.net/ShuSheng0007/article/details/89642945)
3. [Git版本控制：Git分支处理](https://blog.csdn.net/pipisorry/article/details/46958699)
4. [廖雪峰git教程](https://www.liaoxuefeng.com/wiki/896043488029600)
5. [Git HEAD detached from XXX 解决](https://blog.csdn.net/qq_43356428/article/details/98532158)
6. [Git Reset 三种模式](https://www.jianshu.com/p/c2ec5f06cf1a)
7. [git stash 用法总结和注意点](https://www.cnblogs.com/zndxall/archive/2018/09/04/9586088.html)