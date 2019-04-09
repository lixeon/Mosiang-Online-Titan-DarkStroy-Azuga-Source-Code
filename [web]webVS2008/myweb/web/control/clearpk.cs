namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class clearpk : UserControl
    {
        protected Button btnclear;
        protected DropDownList ddchalist;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected TextBox tbpk;

        private void btnclear_Click(object sender, EventArgs e)
        {
            int useridx = int.Parse(base.Session["useridx"].ToString());
            int chaidx = int.Parse(this.ddchalist.SelectedValue.ToString());
            int pknum = Math.Abs(int.Parse(this.tbpk.Text.ToString()));
            int needgold = int.Parse(base.Application["game.clearpkgold"].ToString());
            string str = new WebLogic().clearpk(base.Session["userid"].ToString(), useridx, chaidx, pknum, needgold);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnclear.Click += new EventHandler(this.btnclear_Click);
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
            else if (!this.Page.IsPostBack)
            {
                this.ddchalist.DataSource = new DataProviders().ExecuteSqlDs("select character_idx,character_name+'['+convert(varchar,character_badfame)+'碿]' as character_name from mhgame..tb_character  where substring(character_name,1,1)!='@' and user_idx=" + base.Session["useridx"].ToString(), "ddchalist");
                this.ddchalist.DataTextField = "character_name";
                this.ddchalist.DataValueField = "character_idx";
                this.ddchalist.DataBind();
                for (int i = 0; i < this.ddchalist.Items.Count; i++)
                {
                    this.ddchalist.Items[i].Text = new system().ConvertToBig5(this.ddchalist.Items[i].Text.ToString(), 950);
                }
            }
        }
    }
}
