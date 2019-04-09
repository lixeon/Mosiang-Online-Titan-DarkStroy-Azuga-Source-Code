namespace web.control
{
    using System;
    using System.Data;
    using System.Text.RegularExpressions;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class psinfo : UserControl
    {
        protected Button btnedit;
        protected DataGrid DataGrid1;
        public static int id;
        protected TextBox tbprice;

        private void btnedit_Click(object sender, EventArgs e)
        {
            string str;
            if (!Regex.IsMatch(this.tbprice.Text.ToString(), @"^[+-]?\d*$"))
            {
                str = "請輸入大於0的正整數";
                base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
            }
            else
            {
                int psprice = Math.Abs(int.Parse(this.tbprice.Text.ToString()));
                int pssigngold = int.Parse(base.Application["game.pssigngold"].ToString());
                str = new WebLogic().signpublicsale(base.Session["userid"].ToString(), pssigngold, psprice, id);
                base.Response.Write("<script>alert('" + str + "');</script>");
            }
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
            id = int.Parse(base.Request.QueryString["ID"]);
            DataSet set = new DataSet();
            set = new DataProviders().ExecuteSqlDs("select * from web_psuser where psid=" + id + " order by adddate desc", "DataGrid1");
            for (int i = 0; i < set.Tables[0].Rows.Count; i++)
            {
                string str = set.Tables[0].Rows[i]["userid"].ToString().Substring(0, 1).ToString();
                string str2 = set.Tables[0].Rows[i]["userid"].ToString().Substring(3).ToString();
                set.Tables[0].Rows[i]["userid"] = str + "**" + str2;
            }
            this.DataGrid1.DataSource = set;
            this.DataGrid1.DataBind();
        }
    }
}
