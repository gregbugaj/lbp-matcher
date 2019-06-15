# TODO / Implement

# Noise reduction by using a threashold

# References 

Efficient Spatio-Temporal Local Binary Patterns for Spontaneous Facial Micro-Expression Recognition
https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4438071/


A CO-TRAINING APPROACH TO AUTOMATIC FACE RECOGNITION
https://www.researchgate.net/figure/LBP-face-recognition_fig2_258936085


* http://www.cse.oulu.fi/wsgi/CMV/Downloads/LBPMatlab
* https://www.researchgate.net/publication/264605316_PersianArabic_Handwritten_Digit_Recognition_Using_Local_Binary_Pattern

Matlab extractLBPFeatures
https://www.mathworks.com/help/vision/ref/extractlbpfeatures.html


http://pi-virtualworld.blogspot.com/2014/03/uniform-lbp-features-and-spatial.html

# Catch2 Testing Framework


# Test cases are build with Catch2 test framework

* https://github.com/catchorg/Catch2
* https://github.com/catchorg/Catch2/issues/882
* https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md#where-to-put-it

# Distance measures

* https://dataaspirant.com/2015/04/11/five-most-popular-similarity-measures-implementation-in-python/
* https://stackoverflow.com/questions/6499491/comparing-two-histograms
* http://www.cs.huji.ac.il/~werman/Papers/ECCV2010.pdf

# Some build info

C++17 Required

An official test PPA:

sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-7
gcc-7 --version
Not available in 16.10.

It looks like 17.10 will have it by default: https://launchpad.net/ubuntu/+source/gcc-7

GCC 7 was release in May 2017, so too late for 17.04 main release.
=======
http://www.cse.psu.edu/~rtc12/CSE598G/LKintro_6pp.pdf
https://docs.opencv.org/2.4/modules/imgproc/doc/object_detection.html?highlight=matchtemplate#matchtemplate
https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/template_matching/template_matching.html


Manhatan Distance Instead of Hamming Distance

----------------------------------------------------------


    PIX* pdata = normalize(pix, 0, 0, 0, 0);

    int_t w = pdata->w;
    int_t h = pdata->h;

    // Horizontal * Vertical blocks
    int_t hbin = 1;
    int_t vbin = 1;

    int binsize = 256;
    int bins = hbin * vbin;
    int nbin = bins * binsize;

    int hs = ceil((double) h / (double) hbin);
    int ws = ceil((double) w / (double) vbin);

    /*
    std::cout<<"\n hbin = "<< hbin;
    std::cout<<"\n wbin = "<< vbin;
    std::cout<<"\n bins = "<< bins;
    std::cout<<"\n hs = "<< hs;
    std::cout<<"\n ws = "<< ws;
    std::cout<<"\n nbin = "<< nbin;
     */

    Histogram featureHistogram(nbin);

    PIX* pixUniform = pixCreate(w, h, 8);
    PIX* pixNonUniform = pixCreate(w, h, 8);

    pixSetResolution(pixUniform, 300, 300);
    pixSetResolution(pixNonUniform, 300, 300);

    // Uniform descriptors
    // 0000 0000  (0 Transitions : Uniform)    0x0
    // 1110 0011  (2 Transitions : Uniform)    0xE3
    // 0101 0000  (4 Transitions : NonUniform) 0x50
    // 0000 1010  (4 Transitions : NonUniform) 0xA
    // 0000 1001  (3 Transitions : NonUniform) 0x9

    int index = 0;
    std::cout <<"\n";
    std::set <int> unique;

    int a = 2;

    for (int row = 0; row < vbin; ++row)
    {
        for (int col = 0; col < hbin; ++col)
        {
            index = row * vbin + col;
            int offset = index * binsize;
            int ystart = row * hs;
            int xstart = col * ws;

//          std::cout<<" indexs = "<< index << " pos : " << ystart << ":"<<ystart << ", offset : " << offset << std::endl;
            for (int_t y =  std::max(1, ystart), yend = std::min(ystart + hs, h - 1); y < yend; ++y)
            {
                for (int_t x = std::max(1, xstart), xend = std::min(xstart + ws, w - 1); x < xend; ++x)
                {
                    l_int32 p0 = pixAtGetSan(pdata, x,     y    , w , h); // Center
                    l_int32 p1 = pixAtGetSan(pdata, x - 1, y - 1, w , h);
                    l_int32 p2 = pixAtGetSan(pdata, x,     y - 1, w , h);
                    l_int32 p3 = pixAtGetSan(pdata, x + 1, y - 1, w , h);
                    l_int32 p4 = pixAtGetSan(pdata, x + 1, y    , w , h);
                    l_int32 p5 = pixAtGetSan(pdata, x + 1, y + 1, w , h);
                    l_int32 p6 = pixAtGetSan(pdata, x    , y + 1, w , h);
                    l_int32 p7 = pixAtGetSan(pdata, x - 1, y + 1, w , h);
                    l_int32 p8 = pixAtGetSan(pdata, x - 1, y    , w , h);

                    l_uint8 m =  (p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8) / 9;

                    l_uint8 out = 0;
                    out += sign(p1, p0 + a) * std::pow(2, 0);
                    out += sign(p2, p0 + a) * std::pow(2, 1);
                    out += sign(p3, p0 + a) * std::pow(2, 2);
                    out += sign(p4, p0 + a) * std::pow(2, 3);
                    out += sign(p5, p0 + a) * std::pow(2, 4);
                    out += sign(p6, p0 + a) * std::pow(2, 5);
                    out += sign(p7, p0 + a) * std::pow(2, 6);
                    out += sign(p8, p0 + a) * std::pow(2, 7);

        /*
                    l_uint8  out = 0;
                    out |=   ((p0 > p1) << 7)
                           | ((p0 > p2) << 6)
                           | ((p0 > p3) << 5)
                           | ((p0 > p4) << 4)
                           | ((p0 > p5) << 3)
                           | ((p0 > p6) << 2)
                           | ((p0 > p7) << 1)
                           | ((p0 > p8) << 0);
*/
                    // Check if the bit string is a uniform pattern
                    auto uniform = false;
                    auto tran = transition_lbp(out);

                    if(tran <= 2)
                        uniform = true;

                    if(false)
                    {
                        std::bitset<CHAR_BIT * sizeof(byte_t)> bs(out);
                        std::bitset<CHAR_BIT * sizeof(byte_t)> bsv(m);

                        std::cout << "pos [" << std::dec << std::setw(4) << x <<"," << std::setw(4) << y << "] " << (int)p0 << " :: "<<(int) m << " 0x"

                                  << std::setw(8) << std::hex  << (int) out << " : "
                                  << std::setw(4) << std::dec << (int)out << " :: " << bs <<  "  variance " <<  bsv <<" trans : " <<tran<< std::endl;
                    }

                    ++featureHistogram.bins[offset + static_cast<int>(out)];

                    if(uniform)
                    {
                        unique.insert((unsigned int)out);
                        //std::cout<< (unsigned int) out <<"|";
                        pixAtSet(pixUniform, x, y, out);
                    }
                    else
                    {
                        pixAtSet(pixNonUniform, x, y, out);
                    }
                }
            }

            if(false)
            {
                BOX*  box    = boxCreate(xstart,ystart,ws, hs);
                PIX*  snip   = pixClipRectangle(pix, box, NULL);

                char f[255];
                sprintf(f, "/tmp/lbp-matcher/box-%d-%d.png", row, col);
                pixWritePng(f, snip, 0);
                boxDestroy(&box);
                pixDestroy(&snip);
            }
        }
    }

    pixWritePng("/home/gbugaj/share/devbox/data1/lbp-uniform.png", pixUniform, 1);
    pixWritePng("/home/gbugaj/share/devbox/data1/lbp-nonuniform.png", pixNonUniform, 1);

    pixDestroy(&pdata);
    pixDestroy(&pixUniform);
    pixDestroy(&pixNonUniform);

    //std::cout<<" \n Set : " << unique.size();
    //std::cout<<" Histogram : \n " << featureHistogram;

    /*
       std::cout<< " START \n ";
    for(int i =0, s = featureHistogram.size; i< s; ++i)
    {
        std::cout<< std::dec<< featureHistogram.bins[i] << "\n";
    }
     */
    return featureHistogram;