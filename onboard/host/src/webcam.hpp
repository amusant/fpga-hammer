
/*  Taken from V4L2 video capture example http://linuxtv.org/downloads/v4l-dvb-apis/capture-example.html */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))


enum io_method {
        IO_METHOD_READ,
        IO_METHOD_MMAP,
        IO_METHOD_USERPTR,
};

struct buffer {
        void   *start;
        size_t  length;
};
static void errno_exit(const char *s)
{
        fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
        exit(EXIT_FAILURE);
}

static int xioctl(int fh, int request, void *arg)
{
        int r;

        do {
                r = ioctl(fh, request, arg);
        } while (-1 == r && EINTR == errno);

        return r;
}

class webcam {
	unsigned int    no_of_buffers;
	int              fd = -1;
	unsigned int     n_buffers;
	int              out_buf;
	int              force_format;
	int              frame_count = 70;
	char 		*dev_name;
	public:
	buffer          *buffers;
	webcam(char * dev_name,unsigned int n) {
        	struct stat st;
		// open  device
	
        	if (-1 == stat(dev_name, &st)) {
        	        fprintf(stderr, "Cannot identify '%s': %d, %s\n",
        	                 dev_name, errno, strerror(errno));
        	        exit(EXIT_FAILURE);
        	}

        	if (!S_ISCHR(st.st_mode)) {
        	        fprintf(stderr, "%s is no device\n", dev_name);
        	        exit(EXIT_FAILURE);
        	}

        	fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

        	if (-1 == fd) {
        	        fprintf(stderr, "Cannot open '%s': %d, %s\n",
        	                 dev_name, errno, strerror(errno));
        	        exit(EXIT_FAILURE);
        	}
		no_of_buffers=n;
        	init_device();
        	start_capturing();
	       
	};
	void init_device(void)
	{
	        struct v4l2_capability cap;
	        struct v4l2_cropcap cropcap;
	        struct v4l2_crop crop;
	        struct v4l2_format fmt;
	        unsigned int min;
	
	        if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) {
	                if (EINVAL == errno) {
	                        fprintf(stderr, "%s is no V4L2 device\n",
	                                 dev_name);
	                        exit(EXIT_FAILURE);
	                } else {
	                        errno_exit("VIDIOC_QUERYCAP");
	                }
	        }
	
	        if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
	                fprintf(stderr, "%s is no video capture device\n",
	                         dev_name);
	                exit(EXIT_FAILURE);
	        }
	
	
	
	        /* Select video input, video standard and tune here. */
	
	
	        CLEAR(cropcap);
	
	        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	
	        if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
	                crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	                crop.c = cropcap.defrect; /* reset to default */
	
	                if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) {
	                        switch (errno) {
	                        case EINVAL:
	                                /* Cropping not supported. */
	                                break;
	                        default:
	                                /* Errors ignored. */
	                                break;
	                        }
	                }
	        } else {
	                /* Errors ignored. */
	        }
	//----------------------------------------------------------------------------
	//
	//
	//
	//
	//
	//
	//	struct v4l2_fmtdesc vid_fmtdesc;    /* Enumerated video formats supported by the device */
	//     memset(&vid_fmtdesc, 0, sizeof(vid_fmtdesc));
	//     vid_fmtdesc.index = 0;
	//     char *buf_types[] = {"VIDEO_CAPTURE","VIDEO_OUTPUT", "VIDEO_OVERLAY"}; /* Conversion between enumerated type & english */
	//     char *flags[] = {"uncompressed", "compressed"};  
	//     fprintf(stdout, "\nDiscovering supported video formats:\n");
	//
	//     /* For each of the supported v4l2_buf_type buffer types */
	//     for (vid_fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; vid_fmtdesc.type < V4L2_BUF_TYPE_VIDEO_OVERLAY; vid_fmtdesc.type++)
	//     {
	//         /* Send the VIDIOC_ENUM_FM ioctl and print the results */
	//         while( ioctl( fd, VIDIOC_ENUM_FMT, &vid_fmtdesc ) == 0 )
	//         {
	//
	//             /* We got a video format/codec back \*/
	//             fprintf(stdout,"VIDIOC_ENUM_FMT(%d, %s)\n", vid_fmtdesc.index, buf_types[vid_fmtdesc.type-1]);
	//             fprintf(stdout, "  index        :%d\n", vid_fmtdesc.index);
	//             fprintf(stdout, "  type         :%s\n", buf_types[vid_fmtdesc.type-1]);
	//             fprintf(stdout, "  flags        :%s\n", flags[vid_fmtdesc.flags]);
	//             fprintf(stdout, "  description  :%s\n", vid_fmtdesc.description);
	//
	//             /* Convert the pixelformat attributes from FourCC into 'human readable' format */
	//             fprintf(stdout, "  pixelformat  :%c%c%c%c\n",
	//                                vid_fmtdesc.pixelformat & 0xFF, (vid_fmtdesc.pixelformat >> 8) & 0xFF,
	//                                (vid_fmtdesc.pixelformat >> 16) & 0xFF, (vid_fmtdesc.pixelformat >> 24) & 0xFF);           
	//
	//             /* Increment the index */
	//             vid_fmtdesc.index++;
	//
	//         }
	//     }
	//----------------------------------------------------------------------------
	
	        CLEAR(fmt);
	
	        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	        if (force_format) {
	                fmt.fmt.pix.width       = 544;
	                fmt.fmt.pix.height      = 288;
	                fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	                fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	
	                if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
	                        errno_exit("VIDIOC_S_FMT");
	
	                /* Note VIDIOC_S_FMT may change width and height. */
	        } else {
	                /* Preserve original settings as set by v4l2-ctl for example */
	                if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt))
	                        errno_exit("VIDIOC_G_FMT");
	        }
	
	        /* Buggy driver paranoia. */
	        min = fmt.fmt.pix.width * 2;
	        if (fmt.fmt.pix.bytesperline < min)
	                fmt.fmt.pix.bytesperline = min;
	        min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
	        if (fmt.fmt.pix.sizeimage < min)
	                fmt.fmt.pix.sizeimage = min;
	
	        init_mmap();
	
	};
	void init_mmap(void)
	{
	        struct v4l2_requestbuffers req;
	
	        CLEAR(req);
	
	        req.count = no_of_buffers;
	        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	        req.memory = V4L2_MEMORY_MMAP;
	
	        if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
	                if (EINVAL == errno) {
	                        fprintf(stderr, "%s does not support "
	                                 "memory mapping\n", dev_name);
	                        exit(EXIT_FAILURE);
	                } else {
	                        errno_exit("VIDIOC_REQBUFS");
	                }
	        }
	
	        if (req.count < 2) {
	                fprintf(stderr, "Insufficient buffer memory on %s\n",
	                         dev_name);
	                exit(EXIT_FAILURE);
	        }
	
	        buffers = (buffer *)calloc(req.count, sizeof(*buffers));
	
	        if (!buffers) {
	                fprintf(stderr, "Out of memory\n");
	                exit(EXIT_FAILURE);
	        }
	
	        for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
	                struct v4l2_buffer buf;
	
	                CLEAR(buf);
	
	                buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	                buf.memory      = V4L2_MEMORY_MMAP;
	                buf.index       = n_buffers;
	
	                if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
	                        errno_exit("VIDIOC_QUERYBUF");
	
	                buffers[n_buffers].length = buf.length;
	                buffers[n_buffers].start =
	                        mmap(NULL /* start anywhere */,
	                              buf.length,
	                              PROT_READ | PROT_WRITE /* required */,
	                              MAP_SHARED /* recommended */,
	                              fd, buf.m.offset);
	
	                if (MAP_FAILED == buffers[n_buffers].start)
	                        errno_exit("mmap");
	        }
	};
	void start_capturing(void)
	{
	        unsigned int i;
	        enum v4l2_buf_type type;
	
	                for (i = 0; i < n_buffers; ++i) {
	                        struct v4l2_buffer buf;
	
	                        CLEAR(buf);
	                        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	                        buf.memory = V4L2_MEMORY_MMAP;
	                        buf.index = i;
	
	                        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
	                                errno_exit("VIDIOC_QBUF");
	                }
	                type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	                if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
	                        errno_exit("VIDIOC_STREAMON");
	
	};
	int read_frame(void)
	{
	        struct v4l2_buffer buf;
	
	                CLEAR(buf);
	
	                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	                buf.memory = V4L2_MEMORY_MMAP;
	
	                if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
	                        switch (errno) {
	                        case EAGAIN:
	                                return -1;
	
	                        case EIO:
	                                /* Could ignore EIO, see spec. */
	
	                                /* fall through */
	
	                        default:
	                                errno_exit("VIDIOC_DQBUF");
	                        }
	                }
	
	                assert(buf.index < n_buffers);
	
	                //process_image(buffers[buf.index].start, buf.bytesused);
	
	                if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
	                        errno_exit("VIDIOC_QBUF");
	        return buf.index;
	};
	void uninit_device(void)
	{
	        unsigned int i;
	
	                for (i = 0; i < n_buffers; ++i)
	                        if (-1 == munmap(buffers[i].start, buffers[i].length))
	                                errno_exit("munmap");
	        free(buffers);
	};
	void close_device(void)
	{
	        if (-1 == close(fd))
	                errno_exit("close");
	
	        fd = -1;
	};
	~webcam(){
		 uninit_device();
		 close_device();
	};
};
