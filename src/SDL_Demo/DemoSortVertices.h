class CDemoSortVertices : public CBaseDemo {
    Polygon2D poly;
    Polygon2D poly2;
    PlaneSet2D planes;
    bool bSortVertices;
    bool bDrawRaw;
    bool bDrawConvex;

public:
    CDemoSortVertices();

    virtual void onDemoInit();
    virtual const char* getName() const;
    virtual void onMouseEvent(int x, int y, int button, bool bDown);
    virtual bool onQuit();
    virtual void runFrame();
	virtual void processMyEvent(int code);
};

