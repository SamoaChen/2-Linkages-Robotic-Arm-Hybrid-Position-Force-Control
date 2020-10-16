import time

def waiting(start,division):
    end=time.perf_counter()
    duration=end-start
    duration_desired=1/division
    if duration<duration_desired:
        time.sleep(duration_desired-duration)
