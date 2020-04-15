# zeek repo depends on other 15 repos.
# terminal repos are not changed at all, so we just mirror them to our github.
# 1. clone the upstream to local repo
#    git clone git@github.com:zeek/zeek-docs.git
# 2. git fetch --prune 
# 3. directly push it to internal repo
#    git push --prune git@github.icebrg.io:icebrgio/zeek-docs.git +refs/remotes/origin/*:refs/heads/* +refs/tags/*:refs/tags/*
# 
# terminal repos: 
# zeek-docs             git@github.com:zeek/zeek-docs.git
# paraglob              git@github.com:zeek/paraglob.git
# binpac                git@github.com:zeek/binpac.git
# btest                 git@github.com:zeek/btest.git
# zeek-netcontrol       git@github.com:zeek/zeek-netcontrol.git
# bifcl                 git@github.com:zeek/bifcl.git
# cmake                 git@github.com:zeek/cmake.git
# clang-cindex-python3  git@github.com:wjakob/clang-cindex-python3.git

terminal_repos=(zeek-docs paraglob binpac btest zeek-netcontrol bifcl cmake clang-cindex-python3)


# 2nd level nodes that take terminal nodes as dependencies.
# mirror from upstream
# fix .gitmodules 
# update submodules to tip of master so that it has its new .gitmodules pointed to internal repos.


# 3+ level nodes that take non-terminal nodes as dependencies.

# non-terminal repos which depends on other repos but not modified by us.
# only change we made is .gitmodules.
# otherwise we take all upstream provides.
# we mirror them first, then fix .gitmodules file to point to internal mirrored repos.
# 1. clone the internal repo
#    git clone --recursive git@github.icebrg.io:icebrgio/zeek-aux.git
# 2. setup upstream repo
#    git remote add upstream git@github.com:zeek/zeek-aux.git
# 3. fetch all remotes
#    git fetch --prune --all
# 4. checkout the branch with local changes (usually master branch), to be used to merge again.
#    git checkout --track origin/master
# 5. overwrites origin with upstream (losing changes made on origin )
#    git push --prune origin +refs/remotes/upstream/*:refs/heads/* +refs/tags/*:refs/tags/*
# 6. pull from origin again to mix latest origin with local repo (the previous data)
#    git pull
# 7. the only file that may require (if upstream changes is after our local change in time) manual merge is .gitmodules. solve the conflictiono using ours.
#    git checkout --ours .gitmodules
# 8. add changes to index
#    git add -u
# 9. commit to local repo
#    git commit -m "sync with upstream"
# 10. push changes to origin.
#    git push

# cautiono: log history on .gitmodules after our local change history will be lost. which is fine with us.


# these repos are: 
# zeek-aux              git@github.com:zeek/zeek-aux.git cmake
# trace-summary         git@github.com:zeek/trace-summary.git cmake
# capstats              git@github.com:zeek/capstats.git cmake
# pysubnettree          git@github.com:zeek/pysubnettree.git cmake
# pybind11              git@github.com:pybind/pybind11.git clang-cindex-python3
# broker                git@github.com:zeek/broker.git zeek-3rdparty, pybind11
# zeek-3rdparty         git@github.com:zeek/zeek-3rdparty.git actor-framework
# actor-framework       git@github.com:zeek/actor-framework.git pybind11

# repos we modify, these we cannot mirror, we have to merge manually.
# zeekctl               git@github.com:zeek/zeekctl.git trace-summary, capstats, pysubnettree
# zeek
