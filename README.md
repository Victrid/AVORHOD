# AVORHOD

AVORHOD is a computer program that provides Analysis and Visualization of Online Ride-Hailing Order Data. By processing, classifying and visualising the start, end, time and price information of each order in Online Ride-Hailing's order data, AVORHOD helps data analysis experts to better find the relationship between demand and time distribution and helps Online Ride-Hailing's algorithm designers to further optimise The AVORHOD program helps data analysis experts to better find the relationship between demand and time, and helps the algorithm designers at Online Ride-Hailing to further optimise their algorithms.

The program solves large-scale data processing problems that are difficult to handle with conventional Excel and Python scripts through sequential execution of queues based on SQLite data structures, and provides a better system of visualisation and analysis tools.

## Build and Run
Build it from source:

```
cmake .
make
```

Then run the excutable:

```
./avorhod
```

The report can be found in `AVORHOD.pdf`. You can generate this file from `AVORHOD.tex` in the `src` directory.

See `intro_video.mp4` for usage and description.

The video can also be viewed on [Bilibili](https://www.bilibili.com/video/BV1m64y1o7Ns/).

## License

The project is licensed under GPLv3+.
