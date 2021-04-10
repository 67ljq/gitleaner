# 常用工具





[TOC]

## 音视频工具软件

- 7yuv

  一款查看 yuv 图片的软件

- CoolEdit

  一款查看分析 音频的软件

### yuv图片

- [常用图像像素格式 NV12、NV2、I420、YV12、YUYV](https://blog.csdn.net/cgwang_1580/article/details/79595958)

## ffmpeg

- **ffmpeg ffplay 播放.pcm .g711a .g711u**

```
ffplay -i test.pcm  -f s16le  -ac 1  -ar 8000
ffplay -i test.g711a  -f alaw  -ac 1  -ar 8000
ffplay -i test.g711u  -f mulaw  -ac 1  -ar 8000
```

- **ffmpeg改变jpg,png,webp图片大小**

```
ffmpeg -i input.jpg -vf scale=320:240 output_320x240.jpg
ffmpeg -i input.png -vf scale=320:240 output_320x240.png
ffmpeg -i input.webp -vf scale=320:240 output_320x240.webp
```

**参考：**

- [ffmpeg ffplay 播放.pcm .g711a .g711u](https://blog.csdn.net/qq_38795209/article/details/107414626)
- [ffmpeg改变jpg,png,webp图片大小](https://www.cnblogs.com/freedom-try/p/13991696.html)

## mysql

- 安装

  [免安装版的Mysql](https://www.cnblogs.com/winton-nfs/p/11524007.html#_caption_0)