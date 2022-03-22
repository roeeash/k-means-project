import time
from invoke import task, call



# Same as: @task(pre=[clean]) - can only work on 'pre=' parameter
@task
def build(c):
    c.run("python3.8.5 setup.py build_ext --inplace") ### deleted .5 for running
    print("Done building")


@task
def clear(c):
    c.run("clear")


@task(aliases=['del'])
def delete(c):
    c.run("rm mykmeanssp.cpython-38-x86_64-linux-gnu.so")
    print("File Removed!")

@task(build)
def run(c,k,n,Random=True):
    rand_str = "{}".format(Random)
    msg = "python3.8.5 main.py "+k+" "+n+" %s"%rand_str
    c.run(msg)