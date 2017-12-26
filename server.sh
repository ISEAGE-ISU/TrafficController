socat tcp-listen:4444,reuseaddr exec:./a.out,pty,stderr,setsid,sigint,sane
