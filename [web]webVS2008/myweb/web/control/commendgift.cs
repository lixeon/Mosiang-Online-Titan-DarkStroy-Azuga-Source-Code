namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class commendgift : UserControl
    {
        public static int allppl;
        protected Button btnedit;
        public static int gold;
        public static int goldrate;
        public static int money;
        public static int moneyrate;
        public static int resetppl;
        protected TextBox tbcommendtext;

        private void btnedit_Click(object sender, EventArgs e)
        {
            int commendreborn = int.Parse(base.Application["game.commendreborn"].ToString());
            string str = new WebLogic().commendgift(base.Session["userid"].ToString(), moneyrate, goldrate, commendreborn);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
            this.tbcommendtext.Text = base.Application["commendtext"].ToString().Replace("$uid", base.Session["userid"].ToString());
            allppl = int.Parse(new DataProviders().ExecScalarOne("select count(*) from mhcmember..chr_log_info  where webcommendid='" + base.Session["userid"].ToString() + "'"));
            resetppl = int.Parse(new DataProviders().ExecScalarOne("select count(*) from mhcmember..chr_log_info  where webcommendgift=0 and webcommendid='" + base.Session["userid"].ToString() + "' and propid in (select user_idx from mhgame..tb_character    where webchareset>=" + base.Application["game.commendreborn"].ToString() + ")"));
            moneyrate = int.Parse(base.Application["game.commendmoney"].ToString());
            goldrate = int.Parse(base.Application["game.commendgold"].ToString());
            money = resetppl * moneyrate;
            gold = resetppl * goldrate;
        }
    }
}
