class YIQModifier : public Modifier
{
public:
	float mY, mI, mQ;
	int mOnce;

	virtual void serialize(FILE * f)
	{
		write(f, mY);
		write(f, mI);
		write(f, mQ);
	}

	virtual void deserialize(FILE * f)
	{
		read(f, mY);
		read(f, mI);
		read(f, mQ);
	}

	virtual int gettype()
	{
		return MOD_YIQ;
	}

	YIQModifier()
	{
		mY = mI = mQ = 0;
		mOnce = 0;
	}

	virtual int ui()
	{

		int ret = 0;

		ImGui::PushID(mUnique);

		if (!mOnce)
		{
			ImGui::OpenNextNode(1);
			mOnce = 1;
		}

		if (ImGui::CollapsingHeader("YIQ Modifier"))
		{
			ret = common();

			complexsliderfloat("Y (brightness)", &mY, -1, 1, 0, 0.001f);
			complexsliderfloat("I (blue-red)", &mI, -1, 1, 0, 0.001f);
			complexsliderfloat("Q (green-purple)", &mQ, -1, 1, 0, 0.001f);
		}
		ImGui::PopID();

		return ret;
	}

	virtual void process()
	{
		int c;
		for (c = 0; c < gDevice->mXRes * gDevice->mYRes; c++)
		{
			float r = gBitmapProcFloat[c * 3 + 2];
			float g = gBitmapProcFloat[c * 3 + 1];
			float b = gBitmapProcFloat[c * 3 + 0];

			float y = 0.299f * r + 0.587f * g + 0.114f * b;
			float i = 0.596f * r - 0.274f * g - 0.322f * b;
			float q = 0.211f * r - 0.523f * g + 0.312f * b;

			y += mY;
			i += mI;
			q += mQ;

			r = y + 0.956f * i + 0.621f * q;
			g = y - 0.272f * i - 0.647f * q;
			b = y - 1.106f * i + 1.703f * q;

			if (mB_en) gBitmapProcFloat[c * 3 + 0] = b;
			if (mG_en) gBitmapProcFloat[c * 3 + 1] = g;
			if (mR_en) gBitmapProcFloat[c * 3 + 2] = r;
		}
	}
};
